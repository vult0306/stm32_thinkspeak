# Example of STM32 Client periodically send data to ThinkSpeak server

STM32
  - Collects sensor data every 2s and store to a buffer
  - Stay in Low-Power mode when not collecting data
  - When the buffer is full, send the whole buffer to ESP32 via UART

ESP32
  - When received data from STM32, send to ThinkSpeak server

## Data logger on Thinkspeak server
* Temperature:

    ![image](https://github.com/user-attachments/assets/b2252287-02c8-46d2-9512-236681e30963)

* Humidity:

    ![image](https://github.com/user-attachments/assets/4f6d61a2-17bd-42af-91ef-201dea8d9f12)


## Development Environment Setup
### 1. Install esp-idf release v5.3 at (https://github.com/espressif/esp-idf/tree/release/v5.3)
### 2. Setup ThinkSpeak API key
* Execute "make menuconfig" to enter the configuration screen, navigate to "ThinkSpeak API Key Configuration" screen

  ![image](https://github.com/user-attachments/assets/dfeac2c7-eb22-48ad-90e4-49636d53fe67)

* Enter your API key

  ![image](https://github.com/user-attachments/assets/da1d8683-02be-48c1-b4a4-b4ee1126f3a2)


* The ThinkSpeak API Key can be take from thinkspeak when you create a channel
  
  ![image](https://github.com/user-attachments/assets/7d2703a3-3064-42d3-9736-89389859430f)



### 3. Setup Wifi
  * Navigate to configuration screen to config your wifi SSID and PASSSWORD

  ![image](https://github.com/user-attachments/assets/5b24506f-61c0-4120-8187-da5f4f636912)


  ![image](https://github.com/user-attachments/assets/80dde2e5-eb2c-4d13-a5e7-56e70626537a)



### 4. STM32 Setup
* Install STM32CubeIDE and Import the project to your workspace
* Compile and Run


### 5. Hardware Setup
* Connect STM32 PD8 to ESP32 IO5 for UART communication

  ![image](https://github.com/user-attachments/assets/7c866d19-a44a-4271-8830-bdcc32f8802d)


### 6. ESP32 Compile and Run

  ```idf.py build flash monitor -p /dev/ttyUSBx```

### 7. Power Consumption Measurement
* In Normal mode, it consume ~6.94 mA

![image](https://github.com/user-attachments/assets/a497a268-f43b-4fd4-bf5c-9c0eec126215)


* In Low Power mode, it consume ~4.30 mA

![image](https://github.com/user-attachments/assets/ce91ca3d-5165-46e0-939e-08e5dc268e23)


### 8. For improvement:
* Using UART with DMA to save CPU cycle
* Adding CRC check for message between STM32 & ESP32
* Adding Network Manager to manage wifi connection (connect, disconnect, retry...)
