## Project Details:
- Project name : `iotics-switcher`
- Version : v1.0 - (DEVELOPEMENT PHASE - ALPHA)
- Description : IOT based lights-on-off controller.
- Documentation date : 18-04-2018 .
- Documented by : Karthikeyan.p - product engineer (2018), karthik@iotics.io
- Developers :
  - Hardware section :
    - Schematics designer : S.Naveenkumar - product engineer, Naveen@iotics.io
	- PCB designer : S.Naveenkumar - product engineer, Naveen@iotics.io
  - Software section :
    - ESP8266 Programmer :  Karthikeyan.p - product engineer, karthik@iotics.io
    - ATMEL328pu Programmer : Karthikeyan.p - product engineer, karthik@iotics.io

## Physical Details:
- Board version : iotics_2d_v_1.0
- Board manufactured by : pioneer cirrcuits.
- Board manufactured date : NOVEMBER-2018. 

## Programming Details:
- MCU_1 : esp8266-12e
  - Bootloader : NODE-MCU-1.0 ( ESP-12E module )
  - Upload speed : 115200
  - Flash size : 4M(1M SPIFFS)
  - cpu frequency : 80Mhz
  - software defined programmer : AVRISP MKII

## Hardware pin details:
- MCU_1 : esp8266-12e - Hardware pins:
  ```
  RST - (NO PURPOSE)
  ADC- ACS_712
  EN - (NO PURPOSE)
  [*]	gpio16 - D1_OUT
  [*]	gpio14 - D2_OUT
  [*]	gpio12 - D1_SWITCH
  [*]	gpio13 - D2_SWITCH
  VCC - (NO PURPOSE)
  CSO - (NO PURPOSE)
  MISO - (NO PURPOSE)
  gpio9 - (NO PURPOSE)
  gpio10 - (NO PURPOSE)
  MOSI- (NO PURPOSE)
  SCLK- (NO PURPOSE)
  [*]	TXD0- RX of MCU_2
  [*]	RXD0- TX of MCU_2
  gpio5 - RESET-FACTORY_MODE
  gpio4 - BUZZER
  gpio0 - (NO PURPOSE)
  gpio2 - (NO PURPOSE)
  gpio15 - (NO PURPOSE)
  GND - (NO PURPOSE)
  ```
*( NOTE : basic and minimal circuit is necessory , please see the circuit diagram , the purpose only mentioned above!)*

- MCU_1 : esp8266-12e - {BLYNK} Virtual pins:
    ```
		Device detail
		V2 - rssi

		Device control
		v11 - device 1 on/off button
		V12 - device 2 on/off button
		v19 - master power on button
		V20 - master power off button

		Device schedule
		v21 - scheduler 1 activation button
		V22 - scheduler 2 activation button
		v51 - scheduler 1 timer
		V52 - scheduler 2 timer
		v71 - scheduler 1 device selector
		V72 - scheduler 2 device selector

		Device name
		v31 - device 1 name
		V32 - device 2 name

		Additionals
		v111 - ota
		V99 - terminal
		
		Properatory
		v100 - internal ota
	```	

## IDE deatails :
- IDE name : ARDUINO
- IDE version : 1.8.5

## Other software dependencies detail :
#### ESP8266 :
- esp8266 library details :
  ```
  github : https://github.com/esp8266/Arduino
  version : 2.4.0-rc2
  ```
- blynk library details :
  ```
  github : https://github.com/blynkkk/blynk-library
  version : 0.5.2
  ```
- fauxmaESP library :
  ```
  bitbucket : https://bitbucket.org/xoseperez/fauxmoESP
  version : 2.4.2
  ```