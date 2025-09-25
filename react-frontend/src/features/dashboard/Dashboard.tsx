import React from "react";
import { motion } from "framer-motion";
import { MotionWidget } from "./widgets/MotionWidget.js";
import { TempWidget } from "./widgets/TempWidget.js";
import { HumidityWidget } from "./widgets/HumidityWidget.js";
import DeviceControlPanel from "./widgets/DeviceControlPanel.js";
import SmartRulesStatus from "./widgets/SmartRulesStatus.js";
import MqttConnectionMonitor from "./widgets/MqttConnectionMonitor.js";
import Analytics from "../analytics/Analytics.js";
import Alerts from "../alerts/Alerts.js";

const mockAlerts = [
  { id: 1, message: 'Motion detected in living room' },
  { id: 2, message: 'High humidity detected in kitchen' },
];

const widgetVariants = {
  hidden: { opacity: 0, y: 20 },
  visible: (i: number) => ({
    opacity: 1,
    y: 0,
    transition: { delay: i * 0.2, duration: 0.5 }
  }),
};

const Dashboard: React.FC = () => {
  return (
    <div className="dashboard-container">
      <h1 className="text-3xl font-bold mb-6">Smart Home Dashboard</h1>

      {/* Main Dashboard Layout */}
      <div className="flex flex-col lg:flex-row items-start gap-6 mb-8">
        {/* Left Column - Sensor Widgets */}
        <div className="w-full lg:w-80 space-y-4">
          <motion.div custom={0} initial="hidden" animate="visible" variants={widgetVariants}>
            <MotionWidget
              isMotionDetected={true}
              lastMotionTimestamp="2025-09-22 14:15"
            />
          </motion.div>
          <motion.div custom={1} initial="hidden" animate="visible" variants={widgetVariants}>
            <TempWidget
              temperature={22}
              lastUpdated="2025-09-22 14:20"
            />
          </motion.div>
          <motion.div custom={2} initial="hidden" animate="visible" variants={widgetVariants}>
            <HumidityWidget
              humidity={45}
              lastUpdated="2025-09-22 14:18"
            />
          </motion.div>
        </div>

        {/* Right Column - Analytics */}
        <motion.div custom={3} initial="hidden" animate="visible" variants={widgetVariants} className="flex-1">
          <Analytics />
        </motion.div>
      </div>

      {/* Bottom Row - Device Control, Rules, and MQTT */}
      <div className="flex gap-6 mb-8">
        <motion.div custom={4} initial="hidden" animate="visible" variants={widgetVariants} className="w-full lg:w-80">
          <DeviceControlPanel />
        </motion.div>
        <motion.div custom={5} initial="hidden" animate="visible" variants={widgetVariants} className="flex-1">
          <SmartRulesStatus />
        </motion.div>
        <motion.div custom={6} initial="hidden" animate="visible" variants={widgetVariants} className="w-full lg:w-80">
          <MqttConnectionMonitor />
        </motion.div>
      </div>

      {/* Alerts Section */}
      <motion.div
        className="alerts-section mt-8"
        initial={{ opacity: 0, y: 20 }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ delay: 0.5, duration: 0.5 }}
      >
        <h2 className="text-xl font-semibold mb-4">Recent Alerts</h2>
        <Alerts alerts={mockAlerts} />
      </motion.div>
    </div>
  );
};

export default Dashboard;
