# IoT Doorbell Camera
## Overview

IoT Doorbell Camera is a smart home project designed to transform a traditional doorbell into a connected IoT device equipped with camera capabilities. This project aims to enhance home security by allowing users to monitor their doorstep remotely via a web interface.

## Features

- Picture Capture: Sends a picture of the doorstep directly to a web browser.
- Local Download: Ability to download the taken picture locally on your device from the web server.
- Notification: The web server sends a message that someone is at your door to your mobile device using the Telegram API.
- Cloud Storage Integration: Recorded images are securely stored in the Realtime Database of Firebase.
- Access Control: Ability to grant or deny access remotely, enhancing security and convenience.

## Technologies Used

- Hardware: IoT development board (ESP32 CAM), along with a buzzer, some LED lights for indications and a button (simulating a doorbell).
- Software: ArduinoIDE was used.
- Connectivity/Database: Google Firebase (RealtimeDatabase)  
- Frontend: HTML, CSS, JavaScript (optional for web interface).
