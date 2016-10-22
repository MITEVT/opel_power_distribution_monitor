EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:MITEVT_ANALOG
LIBS:MITEVT_CONTACTORS
LIBS:MITEVT_interface
LIBS:MITEVT_mcontrollers
LIBS:MITEVT_OPTO
LIBS:MITEVT_power
LIBS:MITEVT_REG
LIBS:opel_pdm-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 7400 2900 1850 1250
U 56352A8A
F0 "BACK_PANEL" 60
F1 "back_panel.sch" 60
F2 "CANP" O L 7400 3050 60 
F3 "CANH" B L 7400 3150 60 
F4 "CANL" B L 7400 3250 60 
F5 "CS_SENSE+" O R 9250 3700 60 
F6 "CS_SENSE-" O R 9250 3600 60 
F7 "LV_SENSE+" O R 9250 3400 60 
F8 "LV_SENSE-" O R 9250 3300 60 
$EndSheet
$Sheet
S 2300 2700 1300 1800
U 56353BB7
F0 "FRONT_PANEL" 60
F1 "front_panel.sch" 60
F2 "+5V" O R 3600 2850 60 
F3 "RXD" O R 3600 3050 60 
F4 "TXD" I R 3600 3150 60 
F5 "MCU_RESET_N" O R 3600 3250 60 
F6 "MCU_PROG_N" O R 3600 3350 60 
F7 "LED1" O R 3600 3700 60 
F8 "LED0" O R 3600 3600 60 
F9 "LED3" O R 3600 3900 60 
F10 "LED2" O R 3600 3800 60 
F11 "LED4" O R 3600 4000 60 
F12 "LED5" O R 3600 4100 60 
F13 "LED6" O R 3600 4200 60 
F14 "LED7" O R 3600 4300 60 
$EndSheet
$Sheet
S 4500 4450 1750 1550
U 563E2153
F0 "Micro_Controller" 60
F1 "mcu.sch" 60
F2 "MCU_RESET_N" I L 4500 4900 60 
F3 "RXD" I L 4500 4700 60 
F4 "TXD" O L 4500 4800 60 
F5 "LED7" O L 4500 5950 60 
F6 "LED6" O L 4500 5850 60 
F7 "LED5" O L 4500 5750 60 
F8 "LED4" O L 4500 5650 60 
F9 "LED3" O L 4500 5550 60 
F10 "LED2" O L 4500 5450 60 
F11 "LED1" O L 4500 5350 60 
F12 "LED0" O L 4500 5250 60 
F13 "MCU_PROG_N" I L 4500 5000 60 
F14 "CANL" B R 6250 5100 60 
F15 "CANH" B R 6250 5000 60 
F16 "SCL" O R 6250 5400 60 
F17 "SDA" B R 6250 5500 60 
$EndSheet
$Sheet
S 4850 1850 1200 1100
U 563E6C03
F0 "Power_Supplies" 60
F1 "power.sch" 60
F2 "+12V" I L 4850 2150 60 
F3 "+3V3" O R 6050 2150 60 
F4 "+5V" O L 4850 2600 60 
$EndSheet
Wire Wire Line
	6450 2150 6050 2150
Wire Wire Line
	3600 3050 4350 3050
Wire Wire Line
	4350 3050 4350 4700
Wire Wire Line
	4350 4700 4500 4700
Wire Wire Line
	3600 3150 4300 3150
Wire Wire Line
	4300 3150 4300 4800
Wire Wire Line
	4300 4800 4500 4800
Wire Wire Line
	3600 3250 4250 3250
Wire Wire Line
	4250 3250 4250 4900
Wire Wire Line
	4250 4900 4500 4900
Wire Wire Line
	3600 3600 4050 3600
Wire Wire Line
	4050 3600 4050 5250
Wire Wire Line
	4050 5250 4500 5250
Wire Wire Line
	3600 3700 4000 3700
Wire Wire Line
	4000 3700 4000 5350
Wire Wire Line
	4000 5350 4500 5350
Wire Wire Line
	3600 3800 3950 3800
Wire Wire Line
	3950 3800 3950 5450
Wire Wire Line
	3950 5450 4500 5450
Wire Wire Line
	3600 3900 3900 3900
Wire Wire Line
	3900 3900 3900 5550
Wire Wire Line
	3900 5550 4500 5550
Wire Wire Line
	3600 3350 4200 3350
Wire Wire Line
	4200 3350 4200 5000
Wire Wire Line
	4200 5000 4500 5000
Wire Wire Line
	3600 4000 3850 4000
Wire Wire Line
	3850 4000 3850 5650
Wire Wire Line
	3850 5650 4500 5650
Wire Wire Line
	3600 4100 3800 4100
Wire Wire Line
	3800 4100 3800 5750
Wire Wire Line
	3800 5750 4500 5750
Wire Wire Line
	3600 4200 3750 4200
Wire Wire Line
	3750 4200 3750 5850
Wire Wire Line
	3750 5850 4500 5850
Wire Wire Line
	3600 4300 3700 4300
Wire Wire Line
	3700 4300 3700 5950
Wire Wire Line
	3700 5950 4500 5950
Wire Wire Line
	6250 5000 6850 5000
Wire Wire Line
	6850 5000 6850 3150
Wire Wire Line
	6850 3150 7400 3150
Wire Wire Line
	6250 5100 6900 5100
Wire Wire Line
	6900 5100 6900 3250
Wire Wire Line
	6900 3250 7400 3250
$Comp
L +3.3V #PWR01
U 1 1 56478B8B
P 6450 2000
F 0 "#PWR01" H 6450 1850 50  0001 C CNN
F 1 "+3.3V" H 6450 2140 50  0000 C CNN
F 2 "" H 6450 2000 60  0000 C CNN
F 3 "" H 6450 2000 60  0000 C CNN
	1    6450 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 1450 4700 1450
Wire Wire Line
	6450 2000 6450 2150
$Comp
L +5V #PWR02
U 1 1 5650EA5F
P 3950 2400
F 0 "#PWR02" H 3950 2250 50  0001 C CNN
F 1 "+5V" H 3950 2540 50  0000 C CNN
F 2 "" H 3950 2400 60  0000 C CNN
F 3 "" H 3950 2400 60  0000 C CNN
	1    3950 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 2850 3600 2850
Wire Wire Line
	3950 2400 3950 2850
Connection ~ 3950 2600
Wire Wire Line
	3950 2600 4850 2600
$Sheet
S 7500 4850 1700 1200
U 56C046CA
F0 "I2C Multiplexer and Gas Gauges" 60
F1 "i2c.sch" 60
F2 "SDA" B L 7500 5500 60 
F3 "SCL" I L 7500 5400 60 
F4 "LV_SENSE-" I R 9200 5700 60 
F5 "LV_SENSE+" I R 9200 5600 60 
F6 "CS_SENSE-" I R 9200 5400 60 
F7 "CS_SENSE+" I R 9200 5300 60 
$EndSheet
Wire Wire Line
	7500 5400 6250 5400
Wire Wire Line
	7500 5500 6250 5500
Wire Wire Line
	9250 3700 9400 3700
Wire Wire Line
	9400 3700 9400 5300
Wire Wire Line
	9400 5300 9200 5300
Wire Wire Line
	9250 3600 9450 3600
Wire Wire Line
	9450 3600 9450 5400
Wire Wire Line
	9450 5400 9200 5400
Wire Wire Line
	9250 3400 9500 3400
Wire Wire Line
	9500 3400 9500 5600
Wire Wire Line
	9500 5600 9200 5600
Wire Wire Line
	9250 3300 9550 3300
Wire Wire Line
	9550 3300 9550 5700
Wire Wire Line
	9550 5700 9200 5700
Wire Wire Line
	7400 3050 6850 3050
Wire Wire Line
	6850 3050 6850 1450
Wire Wire Line
	4700 1450 4700 2150
Wire Wire Line
	4700 2150 4850 2150
$EndSCHEMATC
