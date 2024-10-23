# Example of STM32 Client periodically send data to ThinkSpeak server

STM32 collects sensor data and send to ESP via UART/SPI

ESP send the received data to ThinkSpeak server

## ESP32 Setup
* Install esp-idf release v5.3 at (https://github.com/espressif/esp-idf/tree/release/v5.3)
* Setup ThinkSpeak API key

  ![img_v3_02fv_4c8553ff-a3aa-4792-aeb0-e428e07f55dh](https://github.com/user-attachments/assets/c0592312-86d9-45b4-94b8-3897e636b068)

  ![img_v3_02fv_e2ad2e09-7ad6-452e-983a-8067baf26deh](https://github.com/user-attachments/assets/be63c82c-b39f-4a45-99d9-858dda114b92)

* The ThinkSpeak API Key can be take from thinkspeak when you create a channel
  
  ![image](https://github.com/user-attachments/assets/a692f3ad-5076-4683-b593-c402347b5742)


* Setup Wifi
  
  ![img_v3_02fv_e53d1dbc-7695-439e-baa7-4a17524a0a2h](https://github.com/user-attachments/assets/ecf86e17-4dc9-4f60-b91d-21aee9d15330)

  ![img_v3_02fv_9c0c8a59-1e41-412b-9f83-4c45d488d82h](https://github.com/user-attachments/assets/60589ad7-4bc0-4295-9641-150f39936ce7)

## ESP32 Compile and Run
* Execute (ubuntu):
  ```idf.py build flash monitor -p /dev/ttyUSBx```

## STM32 Setup
* Install STM32CubeIDE and Import the project
* Compile and Run

## Hardware Setup
* Connect STM32 PD9 to ESP32 IO5 for UART communication

  ![image](https://github.com/user-attachments/assets/7c866d19-a44a-4271-8830-bdcc32f8802d)
