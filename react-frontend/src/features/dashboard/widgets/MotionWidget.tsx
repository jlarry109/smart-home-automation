import React from "react";
import { FaWalking } from "react-icons/fa";

interface MotionWidgetProps {
  isMotionDetected: boolean;
  lastMotionTimestamp: string;
}

export const MotionWidget: React.FC<MotionWidgetProps> = ({ isMotionDetected, lastMotionTimestamp }) => {
  return (
    <div className={`card ${isMotionDetected ? "border-l-4 border-red-500" : "border-l-4 border-green-500"}`}>
      <div className="flex items-center mb-2">
        <FaWalking className="text-xl mr-2" />
        <div className="card-title">Motion Sensor</div>
      </div>
      <div className={`card-status ${isMotionDetected ? "text-red-600" : "text-green-600"}`}>
        {isMotionDetected ? "Motion detected" : "No motion"}
      </div>
      <div className="card-footer">Last detected: {lastMotionTimestamp}</div>
    </div>
  );
};
