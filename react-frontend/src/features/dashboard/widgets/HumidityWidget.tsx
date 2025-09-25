import React from "react";
import { WiHumidity } from "react-icons/wi";

interface HumidityWidgetProps {
  humidity: number;
  lastUpdated: string;
}

export const HumidityWidget: React.FC<HumidityWidgetProps> = ({ humidity, lastUpdated }) => {
  const getHumidityStatus = (humidity: number) => {
    if (humidity > 60) return { status: "High", color: "text-red-600", borderColor: "border-red-500" };
    if (humidity < 30) return { status: "Low", color: "text-yellow-600", borderColor: "border-yellow-500" };
    return { status: "Normal", color: "text-green-600", borderColor: "border-green-500" };
  };

  const { status, color, borderColor } = getHumidityStatus(humidity);

  return (
    <div className={`card border-l-4 ${borderColor}`}>
      <div className="flex items-center mb-2">
        <WiHumidity className="text-xl mr-2" />
        <div className="card-title">Humidity Sensor</div>
      </div>
      <div className={`card-status ${color}`}>
        {humidity}% - {status}
      </div>
      <div className="card-footer">Last updated: {lastUpdated}</div>
    </div>
  );
};