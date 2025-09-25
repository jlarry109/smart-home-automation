import React from "react";
import { Line } from "react-chartjs-2";
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
} from "chart.js";

ChartJS.register(
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend
);

const TemperatureChart: React.FC = () => {
  const data = {
    labels: ["10 AM", "11 AM", "12 PM", "1 PM", "2 PM"],
    datasets: [
      {
        label: "Temperature (°C)",
        data: [20, 21, 22, 23, 22],
        borderColor: "rgba(239,68,68,1)", // red
        backgroundColor: "rgba(239,68,68,0.2)",
        tension: 0.4,
      },
    ],
  };

  const options = {
    responsive: true,
    maintainAspectRatio: false,
    plugins: {
      legend: { position: "top" as const },
      title: { display: false },
    },
    scales: {
      x: {
        title: { display: true, text: "Time" },
      },
      y: {
        title: { display: true, text: "°C" },
      },
    },
  };

  return <Line data={data} options={options} />;
};

export default TemperatureChart;
