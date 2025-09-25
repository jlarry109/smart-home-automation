const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const mqtt = require('mqtt');
const cors = require('cors');
const AWS = require('aws-sdk');
require('dotenv').config();

// Configure AWS
AWS.config.update({
  region: process.env.AWS_REGION || 'us-east-1'
});

// Use AWS profile if specified
if (process.env.AWS_PROFILE) {
  process.env.AWS_SDK_LOAD_CONFIG = '1';
}
const dynamodb = new AWS.DynamoDB.DocumentClient();

const app = express();
const server = http.createServer(app);
const io = socketIo(server, {
  cors: {
    origin: "http://localhost:5173", // Vite dev server
    methods: ["GET", "POST"]
  }
});

app.use(cors());
app.use(express.json());

// MQTT Configuration - with fallback for development
let mqttClient = null;
const MOCK_MODE = !process.env.MQTT_HOST || process.env.NODE_ENV === 'development';

if (!MOCK_MODE) {
  console.log('Mock mode is OFF; attempting to run on actual MQTT connection');
  try {
    const mqttOptions = {
      host: process.env.MQTT_HOST,
      port: 8883,
      protocol: 'mqtts',
      key: require('fs').readFileSync(process.env.MQTT_KEY_PATH || './certs/pi-node-1.private.key'),
      cert: require('fs').readFileSync(process.env.MQTT_CERT_PATH || './certs/pi-node-1.cert.pem'),
      ca: require('fs').readFileSync(process.env.MQTT_CA_PATH || './certs/AmazonRootCA1.pem'),
      clientId: 'smart-home-backend-' + Math.random().toString(16).substr(2, 8)
    };
    
    mqttClient = mqtt.connect(mqttOptions);
  } catch (error) {
    console.warn('MQTT certificates not found, running in mock mode');
    mqttClient = null;
  }
} else {
  console.log('Running in MOCK MODE - no MQTT connection');
}

if (mqttClient) {
  mqttClient.on('connect', () => {
    console.log('Connected to AWS IoT Core');
    
    // Subscribe to topics your C++ device publishes to
    const topics = [
      'sensors/temperature',
      'sensors/humidity', 
      'sensors/motion',
      'sensors/lux',
      'alerts/temperature',
      'alerts/humidity'
    ];
    
    topics.forEach(topic => {
      mqttClient.subscribe(topic, (err) => {
        if (err) {
          console.error(`Failed to subscribe to ${topic}:`, err);
        } else {
          console.log(`Subscribed to ${topic}`);
        }
      });
    });
  });

  // Handle incoming MQTT messages
  mqttClient.on('message', (topic, message) => {
    try {
      const data = JSON.parse(message.toString());
      console.log(`Received from ${topic}:`, data);
      
      // Forward to all connected React clients
      io.emit('mqtt-data', {
        topic,
        data,
        timestamp: new Date().toISOString()
      });
    } catch (error) {
      console.error('Error parsing MQTT message:', error);
    }
  });
} else {
  // Mock data for development
  setInterval(() => {
    // Temperature data
    io.emit('mqtt-data', {
      topic: 'sensors/temperature',
      data: (20 + Math.random() * 10).toString(),
      timestamp: new Date().toISOString()
    });
    
    // Humidity data
    io.emit('mqtt-data', {
      topic: 'sensors/humidity',
      data: (30 + Math.random() * 40).toString(),
      timestamp: new Date().toISOString()
    });
    
    // Motion data
    io.emit('mqtt-data', {
      topic: 'sensors/motion',
      data: Math.random() > 0.6 ? 'true' : 'false',
      timestamp: new Date().toISOString()
    });
    
    // Lux data
    io.emit('mqtt-data', {
      topic: 'sensors/lux',
      data: (50 + Math.random() * 400).toString(),
      timestamp: new Date().toISOString()
    });
  }, 5000);
}

// Socket.io connection handling
io.on('connection', (socket) => {
  console.log('React client connected:', socket.id);
  
  // Handle device control commands from React
  socket.on('device-control', (command) => {
    console.log('Device control command:', command);
    
    // Build topic based on command
    const { device, room = 'room1', action } = command;
    const topic = `home/${room}/${device}/control`;
    
    if (mqttClient && mqttClient.connected) {
      mqttClient.publish(topic, JSON.stringify(command), (err) => {
        if (err) {
          console.error('Failed to publish command:', err);
          socket.emit('command-error', { error: err.message });
        } else {
          console.log(`Published command to ${topic}`);
          socket.emit('command-success', { topic, command });
        }
      });
    } else {
      console.log(`Mock command sent to ${topic}:`, command);
      socket.emit('command-success', { topic, command, mock: true });
    }
  });
  
  socket.on('disconnect', () => {
    console.log('React client disconnected:', socket.id);
  });
});

// REST API endpoints
app.get('/api/health', (req, res) => {
  res.json({ 
    status: 'ok', 
    mqtt: mqttClient ? mqttClient.connected : false,
    mockMode: MOCK_MODE,
    timestamp: new Date().toISOString()
  });
});

// Historical data endpoints
app.get('/api/sensor-data/:type', async (req, res) => {
  try {
    const { type } = req.params;
    const { hours = 24, device_id = 'pi-node-1' } = req.query;
    
    const endTime = new Date();
    const startTime = new Date(endTime.getTime() - (hours * 60 * 60 * 1000));
    
    const params = {
      TableName: process.env.DYNAMODB_TABLE_NAME || 'SmartHomeTelemetry',
      KeyConditionExpression: 'device_id = :device_id AND #ts BETWEEN :start AND :end',
      FilterExpression: 'sensor = :sensor',
      ExpressionAttributeNames: {
        '#ts': 'timestamp'
      },
      ExpressionAttributeValues: {
        ':device_id': device_id,
        ':start': startTime.toISOString(),
        ':end': endTime.toISOString(),
        ':sensor': type
      },
      ScanIndexForward: true
    };
    
    const result = await dynamodb.query(params).promise();
    res.json({
      data: result.Items,
      count: result.Count
    });
  } catch (error) {
    console.error('DynamoDB query error:', error);
    res.status(500).json({ error: 'Failed to fetch historical data' });
  }
});

// Alerts endpoint
app.get('/api/alerts', async (req, res) => {
  try {
    const { hours = 24, device_id = 'pi-node-1' } = req.query;
    
    const endTime = new Date();
    const startTime = new Date(endTime.getTime() - (hours * 60 * 60 * 1000));
    
    const params = {
      TableName: process.env.DYNAMODB_TABLE_NAME || 'SmartHomeTelemetry',
      KeyConditionExpression: 'device_id = :device_id AND #ts BETWEEN :start AND :end',
      FilterExpression: 'attribute_exists(#rule)',
      ExpressionAttributeNames: {
        '#ts': 'timestamp',
        '#rule': 'rule'
      },
      ExpressionAttributeValues: {
        ':device_id': device_id,
        ':start': startTime.toISOString(),
        ':end': endTime.toISOString()
      },
      ScanIndexForward: false
    };
    
    const result = await dynamodb.query(params).promise();
    res.json({
      alerts: result.Items,
      count: result.Count
    });
  } catch (error) {
    console.error('DynamoDB alerts query error:', error);
    res.status(500).json({ error: 'Failed to fetch alerts' });
  }
});

const PORT = process.env.PORT || 3001;
server.listen(PORT, () => {
  console.log(`Smart Home Backend running on port ${PORT}`);
});