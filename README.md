# the_RANDoms

A Relentless and Realtime Worker Health Analysis System using IoT and Android

A Realtime Safety device and Android Application for the workers of Mining Industry to monitor the hazardous conditions and health analysis to both managers and family members.

IoT has been gradually expanding across different applications which brought a huge attention to the implementation of this project for the mining field, where environments can get a lot noisy and destructive. The main objective of this project is to design and develop a smart helmet and clothing system for mining industry application. The system will provide Realtime monitoring of the hazardous events such as increase in temperature and humidity, release of gasses like Methane, conscious and unconscious state of the miner, removal of helmet of the miner and obstacle damage to the helmet. The programming and troubleshooting will be conducted on mainly two sections, helmet section and control room section that means the system will have a transmitter to transmit necessary data and alerts to the control room or nearest manager and a receiver that can be used by the control room or the managers to grab the data and alerts from the transmitter. On the other side the system also syncs the data to the google firebase database. Apart from this the project also aims to create a system for alerts regarding water rise which is the primary cause of Flooding in mining industries. The above system includes the use of several sensors like IR sensor, Proximity sensor, Humidity Sensor, Gas sensor, and several other modules and microprocessors like node MCU and Arduino. Other problems like Ventilation issues and Haulage problems can be solved by changing the structural construction of mines. The above systems and designs will help stop several hazards and will increase a level of safety and health for the miners, which may attract more number of workers to this field and on the major side it will take an attempt to keep Realtime data for both managers and family members.

This project is about improving/monitoring the health of workers who work in different industries mainly in mines.
Due to the hazardous conditions, the workers work in, an idea was brought into life in the form of a smart workers equipment..
This equipment comprises of parts like helmet, watch, suit and the boot.

The brains of the system is the ESP32 (WROOM32) module, which is the microcontroller with an IC with inbuilt Wi-fi and Bluetooth capabilities.

The sensors used are-
-DHT11 Humidity Sensor
-MQ9 GAS SENSOR
-IR PROXIMITY SENSOR
-MAX30100 PULSE OXIMETER
-HC-SR04 ULTRASONIC SENSOR
-DS18B20 Thermal Probe

Networking is based on Wi-fi which is inbuilt in the ESP32 (WROOM32) module.

Other ideas are also in play, like Radio Frequency using peer to peer connections, SONAR,
TARF-Translational acoustic-rf communication.

**Software part mainly comprises of Arduino IDE which is a compiler to configure microcontrollers.
**For the application development we have used Kodular and for the web page HTML,CSS,JavaScript.

Now comes the data storage and processing stage which is performed by using Google Firebase 
Database which is proven to be a robust tool in collecting and processing of data.

The System uses rechargable batteries but it does not totally rely on these batteries, instead it uses a cycle of charges using piezoelectric plates that works when pressure is applied which furthes creates impulses of electricity. These impulses are ultimately used as cycle of charges to prevent discontinuity of the system.


