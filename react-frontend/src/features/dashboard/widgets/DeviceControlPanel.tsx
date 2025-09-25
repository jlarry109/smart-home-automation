import React, { useState } from "react";
import { FaLightbulb, FaCog } from "react-icons/fa";
import { useMqttData } from "../../../hooks/useMqttData.js";

const DeviceControlPanel: React.FC = () => {
  const { sensorData } = useMqttData();
  const [lightOn, setLightOn] = useState(false);
  const [autoMode, setAutoMode] = useState(true);
  const currentLux = sensorData.lux || 0;

  const handleLightToggle = () => {
    setLightOn(!lightOn);
    // TODO: Send MQTT command to C++ light controller
  };

  const handleModeToggle = () => {
    setAutoMode(!autoMode);
    // TODO: Send MQTT command to enable/disable auto mode
  };

  return (
    <div className="card h-full flex flex-col">
      <div className="flex items-center mb-3">
        <FaCog className="text-lg mr-2" />
        <div className="card-title">Device Control</div>
      </div>
      
      <div className="space-y-2">
        {/* Light Control */}
        <div className="flex items-center justify-between p-2 bg-gray-50 rounded">
          <div className="flex items-center">
            <FaLightbulb className={`text-lg mr-2 ${lightOn ? 'text-yellow-500' : 'text-gray-400'}`} />
            <span className="font-medium">Living Room Light</span>
          </div>
          <button
            onClick={handleLightToggle}
            className={`px-3 py-1 rounded text-sm ${
              lightOn ? 'bg-yellow-500 text-white' : 'bg-gray-300 text-gray-700'
            }`}
          >
            {lightOn ? 'ON' : 'OFF'}
          </button>
        </div>

        {/* Auto Mode Toggle */}
        <div className="flex items-center justify-between p-2 bg-gray-50 rounded">
          <span className="font-medium">Auto Mode</span>
          <button
            onClick={handleModeToggle}
            className={`px-3 py-1 rounded text-sm ${
              autoMode ? 'bg-green-500 text-white' : 'bg-gray-300 text-gray-700'
            }`}
          >
            {autoMode ? 'AUTO' : 'MANUAL'}
          </button>
        </div>

        {/* Current Lux */}
        <div className="p-2 bg-gray-50 rounded">
          <div className="flex justify-between items-center">
            <span className="text-sm font-medium">Lux: {Math.round(currentLux)}</span>
            <span className="text-xs text-gray-500">ON&lt;100 | OFF&gt;300</span>
          </div>
        </div>
      </div>
    </div>
  );
};

export default DeviceControlPanel;