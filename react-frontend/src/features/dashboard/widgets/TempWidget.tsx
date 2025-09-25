import React from "react";
import { FaThermometerHalf } from "react-icons/fa";
import { useMqttData } from "../../../hooks/useMqttData.js";

export const TempWidget: React.FC = () => {
  const { sensorData } = useMqttData();
  const temperature = sensorData.temperature || 22; // fallback to mock data
  const lastUpdated = sensorData.lastUpdated;
  const getTempStatus = (temp: number) => {
    if (temp > 25) return { status: "Warm", color: "text-red-600", borderColor: "border-red-500" };
    if (temp < 18) return { status: "Cool", color: "text-blue-600", borderColor: "border-blue-500" };
    return { status: "Comfortable", color: "text-green-600", borderColor: "border-green-500" };
  };

  const { status, color, borderColor } = getTempStatus(temperature);

  return (
    <div className={`card border-l-4 ${borderColor}`}>
      <div className="flex items-center mb-2">
        <FaThermometerHalf className="text-xl mr-2" />
        <div className="card-title">Temperature Sensor</div>
      </div>
      <div className={`card-status ${color}`}>
        {temperature.toFixed(2)}°C - {status}
      </div>
      <div className="card-footer">Last updated: {new Date(lastUpdated).toLocaleTimeString()}</div>
    </div>
  );
};