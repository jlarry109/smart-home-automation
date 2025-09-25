import React, { useState } from "react";
import { FaCogs } from "react-icons/fa";

interface Rule {
  name: string;
  enabled: boolean;
  threshold: string;
  lastTriggered: string;
  triggerCount: number;
}

const SmartRulesStatus: React.FC = () => {
  const [rules, setRules] = useState<Rule[]>([
    { name: "MotionAtNightRule", enabled: true, threshold: "28 lux", lastTriggered: "14:15", triggerCount: 3 },
    { name: "HighTempRule", enabled: true, threshold: "25°C", lastTriggered: "13:45", triggerCount: 1 },
    { name: "HumiditySpikeRule", enabled: false, threshold: "55%", lastTriggered: "12:30", triggerCount: 0 },
  ]);

  const toggleRule = (index: number) => {
    setRules(prev => prev.map((rule, i) => 
      i === index ? { ...rule, enabled: !rule.enabled } : rule
    ));
    // TODO: Send MQTT command to enable/disable rule
  };

  return (
    <div className="card h-full">
      <div className="flex items-center mb-3">
        <FaCogs className="text-lg mr-2" />
        <div className="card-title">Smart Rules Engine</div>
      </div>
      
      <div className="space-y-2">
        {rules.map((rule, index) => (
          <div key={rule.name} className="p-2 bg-gray-50 rounded">
            <div className="flex items-center justify-between mb-1">
              <span className="text-sm font-medium">{rule.name}</span>
              <button
                onClick={() => toggleRule(index)}
                className={`px-2 py-1 rounded text-xs ${
                  rule.enabled ? 'bg-green-500 text-white' : 'bg-gray-300 text-gray-700'
                }`}
              >
                {rule.enabled ? 'ON' : 'OFF'}
              </button>
            </div>
            <div className="flex justify-between text-xs text-gray-600">
              <span>Threshold: {rule.threshold}</span>
              <span>Triggers: {rule.triggerCount}</span>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
};

export default SmartRulesStatus;