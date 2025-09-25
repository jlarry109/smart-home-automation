import React from "react";
import TemperatureChart from "./charts/TemperatureChart.js";
import HumidityChart from "./charts/HumidityChart.js";
import MotionChart from "./charts/MotionChart.js";

const Analytics: React.FC = () => {
  return (
    <div className="analytics-container flex flex-col md:flex-row items-start gap-6 w-full">
      {/* Temperature */}
      <div className="analytics-card flex-1 p-4 h-64 md:h-80">
        <TemperatureChart />
      </div>

      {/* Humidity */}
      <div className="analytics-card flex-1 p-4 h-64 md:h-80">
        <HumidityChart />
      </div>

      {/* Motion */}
      <div className="analytics-card flex-1 p-4 h-64 md:h-80">
        <MotionChart />
      </div>
    </div>
  );
};

export default Analytics;
