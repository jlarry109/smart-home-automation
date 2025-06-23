# Smart Home Automation System

A Raspberry Pi-based home automation system using C++, MQTT, and AWS IoT Core to control and monitor devices like lights, fans, and cameras.

## Features
- Control GPIO-connected devices via cloud
- Use AWS IoT Core for secure MQTT messaging
- Fallback to local control when offline
- Real-time sensor telemetry (temp, motion, etc.)

## Tech Stack
- C++ (device code)
- AWS IoT Core (cloud MQTT broker)
- AWS Lambda (processing events)
- DynamoDB (telemetry log)
- Raspberry Pi GPIO

## Architecture
[Insert architecture-diagram.png]

## MQTT Topics
| Topic                       | Purpose                      |
|----------------------------|-------------------------------|
| home/room1/light1/control  | Cloud → Device command       |
| home/room1/light1/status   | Device → Cloud status update |
| home/room1/temp            | Device → Cloud temp data     |

## Getting Started
1. Clone repo
2. Set up AWS IoT & download certs
3. Build C++ edge app
4. Connect Raspberry Pi


