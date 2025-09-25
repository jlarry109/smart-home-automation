# Smart Home Backend

Node.js backend server that bridges the C++ IoT system with the React dashboard.

## Features
- MQTT bridge to AWS IoT Core
- Real-time WebSocket communication with React frontend
- REST API for historical data
- Device control command forwarding

## Setup

1. **Install dependencies:**
   ```bash
   npm install
   ```

2. **Configure environment:**
   ```bash
   cp .env.example .env
   # Edit .env with your AWS IoT Core settings
   ```

3. **Add AWS IoT certificates:**
   ```bash
   mkdir certs
   # Copy your AWS IoT certificates to ./certs/
   ```

4. **Start server:**
   ```bash
   npm run dev  # Development with nodemon
   npm start    # Production
   ```

## API Endpoints

- `GET /api/health` - Server and MQTT connection status
- `GET /api/sensor-data/:type` - Historical sensor data

## WebSocket Events

**From React to Backend:**
- `device-control` - Send commands to C++ device

**From Backend to React:**
- `mqtt-data` - Real-time sensor data from C++ device
- `command-success` - Command sent successfully
- `command-error` - Command failed

## MQTT Topics

**Subscribed (from C++ device):**
- `sensors/temperature` - Temperature readings
- `sensors/humidity` - Humidity readings  
- `sensors/motion` - Motion events
- `sensors/lux` - Light sensor readings
- `alerts/temperature` - High temperature alerts
- `alerts/humidity` - Humidity spike alerts

**Published (to C++ device):**
- `home/room1/light1/control` - Light control commands