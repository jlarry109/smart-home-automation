import { useState, useEffect } from 'react';
import { io, Socket } from 'socket.io-client';
import { telemetryApi, TelemetryData, Alert } from '../api/telemetryApi.js';

interface MqttMessage {
  topic: string;
  data: any;
  timestamp: string;
}

interface SensorData {
  temperature: number | null;
  humidity: number | null;
  motion: boolean | null;
  lux: number | null;
  lastUpdated: string;
}

interface HistoricalData {
  temperature: TelemetryData[];
  humidity: TelemetryData[];
  motion: TelemetryData[];
}

export const useMqttData = () => {
  const [socket, setSocket] = useState<Socket | null>(null);
  const [connected, setConnected] = useState(false);
  const [sensorData, setSensorData] = useState<SensorData>({
    temperature: null,
    humidity: null,
    motion: null,
    lux: null,
    lastUpdated: new Date().toISOString()
  });
  const [historicalData, setHistoricalData] = useState<HistoricalData>({
    temperature: [],
    humidity: [],
    motion: []
  });
  const [alerts, setAlerts] = useState<Alert[]>([]);

  useEffect(() => {
    // Connect to backend WebSocket
    const newSocket = io('http://localhost:3001');
    setSocket(newSocket);

    newSocket.on('connect', () => {
      console.log('Connected to backend');
      setConnected(true);
    });

    newSocket.on('disconnect', () => {
      console.log('Disconnected from backend');
      setConnected(false);
    });

    // Handle real-time MQTT data
    newSocket.on('mqtt-data', (message: MqttMessage) => {
      console.log('Received MQTT data:', message);
      
      // Update sensor data based on topic
      if (message.topic === 'sensors/temperature') {
        setSensorData(prev => ({
          ...prev,
          temperature: parseFloat(message.data),
          lastUpdated: message.timestamp
        }));
      } else if (message.topic === 'sensors/humidity') {
        setSensorData(prev => ({
          ...prev,
          humidity: parseFloat(message.data),
          lastUpdated: message.timestamp
        }));
      } else if (message.topic === 'sensors/motion') {
        setSensorData(prev => ({
          ...prev,
          motion: message.data === 'true',
          lastUpdated: message.timestamp
        }));
      } else if (message.topic === 'sensors/lux') {
        setSensorData(prev => ({
          ...prev,
          lux: parseFloat(message.data),
          lastUpdated: message.timestamp
        }));
      }
    });

    return () => {
      newSocket.close();
    };
  }, []);

  // Load historical data
  const loadHistoricalData = async (hours: number = 24) => {
    try {
      const [tempData, humidityData, motionData, alertsData] = await Promise.all([
        telemetryApi.getSensorData('temperature', hours),
        telemetryApi.getSensorData('humidity', hours),
        telemetryApi.getSensorData('motion', hours),
        telemetryApi.getAlerts(hours)
      ]);
      
      setHistoricalData({
        temperature: tempData,
        humidity: humidityData,
        motion: motionData
      });
      setAlerts(alertsData);
    } catch (error) {
      console.error('Failed to load historical data:', error);
    }
  };

  const sendDeviceCommand = (command: any) => {
    if (socket && connected) {
      socket.emit('device-control', command);
    }
  };

  return {
    connected,
    sensorData,
    historicalData,
    alerts,
    sendDeviceCommand,
    loadHistoricalData
  };
};