import React from "react";
import { FaWifi, FaCircle } from "react-icons/fa";

const MqttConnectionMonitor: React.FC = () => {
  const connectionStatus = "connected"; // TODO: Get from MQTT client
  const lastMessage = "14:20:15";
  const publishRate = 12; // messages per minute
  const subscribeRate = 8;

  const getStatusColor = (status: string) => {
    switch (status) {
      case 'connected': return 'text-green-500';
      case 'connecting': return 'text-yellow-500';
      case 'disconnected': return 'text-red-500';
      default: return 'text-gray-500';
    }
  };

  return (
    <div className="card h-full">
      <div className="flex items-center mb-3">
        <FaWifi className="text-lg mr-2" />
        <div className="card-title">MQTT Monitor</div>
      </div>
      
      <div className="space-y-3">
        {/* Connection Status */}
        <div className="flex items-center justify-between">
          <span className="text-sm font-medium">AWS IoT Core</span>
          <div className="flex items-center">
            <FaCircle className={`text-xs mr-1 ${getStatusColor(connectionStatus)}`} />
            <span className="text-sm capitalize">{connectionStatus}</span>
          </div>
        </div>

        {/* Message Rates */}
        <div className="grid grid-cols-2 gap-2 text-xs">
          <div className="bg-gray-50 p-2 rounded">
            <div className="font-medium">Publish</div>
            <div className="text-green-600">{publishRate}/min</div>
          </div>
          <div className="bg-gray-50 p-2 rounded">
            <div className="font-medium">Subscribe</div>
            <div className="text-blue-600">{subscribeRate}/min</div>
          </div>
        </div>

        {/* Last Activity */}
        <div className="text-xs text-gray-600">
          <div>Last message: {lastMessage}</div>
          <div className="mt-1">Topics: alerts/*, home/*</div>
        </div>
      </div>
    </div>
  );
};

export default MqttConnectionMonitor;