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

const HumidityChart: React.FC = () => {
  const data = {
    labels: ["10 AM", "11 AM", "12 PM", "1 PM", "2 PM"],
    datasets: [
      {
        label: "Humidity (%)",
        data: [50, 52, 55, 57, 55, 32, 57, 57, 98, 57, 25, 64],
        borderColor: "rgba(59,130,246,1)", // blue
        backgroundColor: "rgba(59,130,246,0.2)",
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
        title: { display: true, text: "%" },
      },
    },
  };

  return <Line data={data} options={options} />;
};

export default HumidityChart;
