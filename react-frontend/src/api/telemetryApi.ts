const API_BASE_URL = 'http://localhost:3001/api';

export interface TelemetryData {
  device_id: string;
  timestamp: string;
  sensor: string;
  value: number;
  message?: string;
}

export interface Alert {
  device_id: string;
  timestamp: string;
  rule: string;
  message: string;
  sensor: string;
  value: number;
}

export const telemetryApi = {
  // Get historical sensor data
  getSensorData: async (type: string, hours: number = 24): Promise<TelemetryData[]> => {
    const response = await fetch(`${API_BASE_URL}/sensor-data/${type}?hours=${hours}`);
    if (!response.ok) throw new Error('Failed to fetch sensor data');
    const result = await response.json();
    return result.data;
  },

  // Get alerts
  getAlerts: async (hours: number = 24): Promise<Alert[]> => {
    const response = await fetch(`${API_BASE_URL}/alerts?hours=${hours}`);
    if (!response.ok) throw new Error('Failed to fetch alerts');
    const result = await response.json();
    return result.alerts;
  },

  // Health check
  getHealth: async () => {
    const response = await fetch(`${API_BASE_URL}/health`);
    if (!response.ok) throw new Error('Health check failed');
    return response.json();
  }
};