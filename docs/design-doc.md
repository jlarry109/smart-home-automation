#  Smart Home Automation System – Design Document

##  Summary

This project builds a modular smart home automation system using Raspberry Pi (C++), AWS IoT Core, and MQTT to control and monitor home devices like lights, fans, and sensors. The system allows remote interaction, telemetry logging, and scalable multi-device coordination while leveraging modern C++ practices and cloud services.

##  Problem Statement

Most consumer smart home solutions are closed, proprietary, or cloud-locked, limiting flexibility and local control. There is a need for an **open, edge-first smart home framework** that allows:
- Low-latency control
- Local override for safety
- Scalable integration with cloud services
- Secure communication between devices and users

##  Objective

- Use **modern C++** to control GPIO-based devices on Raspberry Pi.
- Communicate securely with AWS IoT using **MQTT**.
- Publish telemetry (e.g., temperature, motion) and subscribe to control topics.
- Handle cloud outages with graceful local fallbacks.
- Provide optional extensions (e.g., camera integration, dashboard, alerts).

##  Proposed Solution

The system consists of:
- **Edge devices** (Raspberry Pi nodes) running modern C++ apps using smart pointers and threads.
- **Cloud messaging** via AWS IoT Core with secure X.509 TLS.
- Optional **event processing** using AWS Lambda.
- Optional **telemetry storage** using DynamoDB.
- A **modular dashboard** built in React (optional) for user interaction.

##  Proposed Design

### Key Components
| Component       | Responsibility                                 |
|----------------|--------------------------------------------------|
| Raspberry Pi    | Device control (lights, sensors) via GPIO       |
| C++ App         | MQTT publish/subscribe, local device logic      |
| AWS IoT Core    | Secure message brokering & device shadow        |
| Lambda (cloud)  | Triggered on alerts (e.g., motion detected)     |
| DynamoDB        | Store temperature, motion, and device logs      |
| Dashboard       | User interface for device monitoring            |

##  High-Level Design Diagram

_Refer to architecture diagram: `docs/architecture-diagram.png`_  
It shows interactions between:
- User → AWS IoT → Pi
- Pi → AWS IoT → Lambda → DynamoDB
- Local feedback loop between GPIO, sensors, and MQTT

##  Flow of Responsibility

1. **Control Flow (User → Device):**
   - User issues command from UI
   - AWS IoT routes MQTT message to device
   - Pi toggles GPIO and publishes status update

2. **Telemetry Flow (Device → Cloud):**
   - Pi reads temperature/motion
   - Publishes to MQTT topics (e.g., `home/room1/temp`)
   - AWS IoT Rule triggers Lambda → logs to DynamoDB

3. **Alerting Flow:**
   - Pi detects motion → publishes to `home/alerts/motion`
   - Lambda triggers alert to user (SMS/email)

##  Error Handling & Failures

| Failure Case                  | Handling Strategy                            |
|------------------------------|-----------------------------------------------|
| Lost Wi-Fi or Cloud Access   | Pi uses local state machine for control       |
| MQTT publish failure         | Retry with backoff, log to local buffer       |
| Sensor read fails            | Publish `null` or omit reading, log warning   |
| TLS/cert expiration          | Certificates rotated & monitored on cloud     |
| Device reboot/power loss     | Reload last known state from device shadow     |


#  Smart Home Automation System – Updated Design Overview (07/07/2025)

This project implements a modular, edge-first smart home automation system using **Raspberry Pi (C++)**, **MQTT**, and **AWS IoT Core**. It simulates real-world conditions using mocks first, applies smart automation rules to sensor data, and controls actuators such as lights based on defined logic. The architecture is designed to be **modern**, **scalable**, and **extensible** — with support for **secure cloud integration**, **telemetry**, and **local control** during cloud outages.

---

##  Core Design Highlights

-  **SOLID C++ design principles**
-  **Mock-first development** for realism and testability
-  **Modular orchestration layer** for smart decision-making
- ️ **Cloud connectivity** for telemetry, analytics, and alerting

---

##  Key Enhancements Since Original Design

| Area         | Original Vision                                       | Current Evolution                                                                 |
|--------------|--------------------------------------------------------|------------------------------------------------------------------------------------|
| **Sensors**  | Basic GPIO-based sensing                               | Modular C++ sensors with mock + real implementations for motion, light, temp, humidity |
| **Rules**    | Manual conditions                                      | Dynamic smart rule engine with plug-and-play rules (e.g., `MotionAtNight`, `HighTemp`) |
| **Logic Flow** | Single flow per sensor                              | Orchestrator layer evaluates sensor states together using customizable rules       |
| **Testing**  | Real devices required                                  | Full simulation loop via mocks (lux spikes, humidity jumps, motion events)        |
| **Light Control** | GPIO only                                        | `LightController` with threshold-based logic, force control, and local failover     |
| **MQTT**     | Bidirectional                                          | Publishing telemetry + subscribing to cloud control topics                         |
| **Cloud**    | AWS IoT only                                           | MQTT + future Lambda alerts + DynamoDB logging                                     |
| **Dashboard**| Planned                                                | React frontend to be built after backend logic stabilizes                         |
| **Resilience**| Stated goal                                          | Local fallback, offline rule evaluation, cert monitoring roadmap                   |

---

## Updated Component Overview

| Component          | Responsibility                                                                 |
|--------------------|----------------------------------------------------------------------------------|
| **Raspberry Pi**    | Edge device running orchestration and control logic                            |
| **C++ Modules**     | Sensor mocks, actuators, rule engine, orchestrator                             |
| **Rules**           | Automation logic (e.g., motion + lux = turn light on)                          |
| **Light Controller**| Decides when to turn relay on/off based on lux or rule overrides               |
| **MQTT Client**     | Publishes sensor data, listens for control updates                             |
| **AWS IoT Core**    | Secure MQTT broker for telemetry and command exchange                          |
| **React Dashboard** | *(Planned)* UI to view live data and send control commands                     |
| **DynamoDB / Lambda**| *(Planned)* Persist alerts and telemetry, trigger notifications               |

---

## Architecture Foundation

- Follows **modern C++20** practices: smart pointers, RAII, modular headers
- Strong **separation of concerns**: sensor, monitor, controller, rules
- Tested using mocks, logs, and condition simulations before hardware deployment
- Designed for **scale**: easy to plug in more sensors, rules, or actuators

---

## Next Milestones

-  **Add local logging** of sensor readings (CSV or SQLite)
-  **Expand smart rule library** (e.g., `HumiditySpikeRule`, `SuddenLuxDropRule`)
-  **Integrate additional actuators** (fan relay, buzzer)
-  **Start Lambda + DynamoDB** cloud analytics pipeline
-  **Build initial React dashboard** with MQTT integration
