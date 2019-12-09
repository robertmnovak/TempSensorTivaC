# TempSensorTivaC
 This project is based around the Tiva C Launchpad(TM4C123GH6PM) and uses the TM4C123GH6PM.h header file based around the CMSIS framework for it.
 
 This project uses I2C to communicate with a BME280 sensor and display its data onto a 128x64 OLED screen. I also incorporated UART into the project to display the data onto a phone
 through the HM-10 Bluetooth module(UART3), or onto a PC(UART0) if the Launchpad is connected to it through USB.
