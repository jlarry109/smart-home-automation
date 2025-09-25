// src/features/alerts/AlertItem.tsx
import React from "react";

interface AlertItemProps {
  message: string;
}

export const AlertItem: React.FC<AlertItemProps> = ({ message }) => {
  return <div className="alert-item">{message}</div>;
};


export default AlertItem;
