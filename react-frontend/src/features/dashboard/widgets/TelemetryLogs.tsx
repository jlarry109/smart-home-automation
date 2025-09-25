import React from "react";
import { FaDatabase } from "react-icons/fa";

interface TelemetryLog {
  id: number;
  timestamp: string;
  sensor: string;
  value: string;
  type: 'temperature' | 'humidity' | 'motion';
}

const mockLogs: TelemetryLog[] = [
  { id: 1, timestamp: "14:20", sensor: "Living Room", value: "22°C", type: "temperature" },
  { id: 2, timestamp: "14:18", sensor: "Kitchen", value: "45%", type: "humidity" },
  { id: 3, timestamp: "14:15", sensor: "Living Room", value: "Motion", type: "motion" },
  { id: 4, timestamp: "14:12", sensor: "Bedroom", value: "21°C", type: "temperature" },
  { id: 5, timestamp: "14:10", sensor: "Bathroom", value: "52%", type: "humidity" },
  { id: 6, timestamp: "14:08", sensor: "Kitchen", value: "No Motion", type: "motion" },
];

const TelemetryLogs: React.FC = () => {
  const getTypeColor = (type: string) => {
    switch (type) {
      case 'temperature': return 'text-red-600';
      case 'humidity': return 'text-blue-600';
      case 'motion': return 'text-green-600';
      default: return 'text-gray-600';
    }
  };

  return (
    <div className="card h-full">
      <div className="flex items-center mb-4">
        <FaDatabase className="text-xl mr-2" />
        <div className="card-title">Recent Telemetry</div>
      </div>
      <div className="flex-1 overflow-y-auto max-h-64">
        <div className="space-y-2">
          {mockLogs.map((log) => (
            <div key={log.id} className="flex justify-between items-center py-2 border-b border-gray-100 last:border-b-0">
              <div className="flex flex-col">
                <span className="text-sm font-medium">{log.sensor}</span>
                <span className="text-xs text-gray-500">{log.timestamp}</span>
              </div>
              <span className={`text-sm font-medium ${getTypeColor(log.type)}`}>
                {log.value}
              </span>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
};

export default TelemetryLogs;