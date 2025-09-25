import React from "react";
import { FaWalking } from "react-icons/fa";
import { useMqttData } from "../../../hooks/useMqttData.js";

export const MotionWidget: React.FC = () => {
  const { sensorData } = useMqttData();
  const isMotionDetected = sensorData.motion || false;
  return (
    <div className={`card ${isMotionDetected ? "border-l-4 border-red-500" : "border-l-4 border-green-500"}`}>
      <div className="flex items-center mb-2">
        <FaWalking className="text-xl mr-2" />
        <div className="card-title">Motion Sensor</div>
      </div>
      <div className={`card-status ${isMotionDetected ? "text-red-600" : "text-green-600"}`}>
        {isMotionDetected ? "Motion detected" : "No motion"}
      </div>
      <div className="card-footer">Last updated: {new Date(sensorData.lastUpdated).toLocaleTimeString()}</div>
    </div>
  );
};
