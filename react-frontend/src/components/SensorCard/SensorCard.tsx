import React from "react";

interface SensorCardProps {
  title: string;
  status: string;
  statusColor: "red" | "green" | "yellow" | "blue";
  footerText?: string;
}

export const SensorCard: React.FC<SensorCardProps> = ({
  title,
  status,
  statusColor,
  footerText,
}) => {
  const statusColors: Record<string, string> = {
    red: "bg-red-100 text-red-700",
    green: "bg-green-100 text-green-700",
    yellow: "bg-yellow-100 text-yellow-700",
    blue: "bg-blue-100 text-blue-700",
  };

  return (
    <div className="bg-white rounded-2xl shadow-md p-4 space-y-3 transition hover:shadow-lg">
      <h3 className="text-lg font-medium">{title}</h3>
      <div className={`inline-block px-3 py-1 rounded-full text-sm font-medium ${statusColors[statusColor]}`}>
        {status}
      </div>
      {footerText && <p className="text-sm text-gray-500">{footerText}</p>}
    </div>
  );
};