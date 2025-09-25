import React from "react";
import { FaExclamationCircle } from "react-icons/fa";

interface Alert {
  id: number;
  message: string;
}

interface AlertsProps {
  alerts: Alert[];
}

const Alerts: React.FC<AlertsProps> = ({ alerts }) => {
  return (
    <>
      {alerts.map(alert => (
        <div key={alert.id} className="alert-item flex items-center gap-2">
          <FaExclamationCircle className="text-red-600" />
          <span>{alert.message}</span>
        </div>
      ))}
    </>
  );
};

export default Alerts;
