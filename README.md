---------------------------------------------------------------------------------------
PSoC-6 (Cypress test)
---------------------------------------------------------------------------------------

Requirements
-------------------
Tool: ModusToolbox IDE <br />
Programming Language: C

Supported Kit
--------------------
PSoC 6 CY8CPROTO-062-4343W

Overview
------------
This example shows how to configure RTC, UART, SAR ADC, Thermistor and Button Interrupt. The essence of the program is to check the date, time and read information from temperature sensor every second. 

Detail overview
--------------------
In the beginning, the program checks the date and time and prints it in the terminal also reads temperature from thermistor and print it in the terminal.These actions repeat every second and every 30 sec. this information is burned to the SD card during the program running. Temperature is set in Celsius by default but you can change it to Fahrenheit by pushing the button (Pin: 0.4).

Attention
----------
