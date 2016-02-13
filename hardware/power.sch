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
LIBS:final_outline-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCP1755 U501
U 1 1 563E6C0D
P 4850 3350
F 0 "U501" H 5500 3350 60  0000 C CNN
F 1 "MCP1755-33" H 5250 3950 60  0000 C CNN
F 2 "Housings_SOT-23_SOT-143_TSOT-6:SOT-23-5" H 4850 3350 60  0001 C CNN
F 3 "" H 4850 3350 60  0000 C CNN
	1    4850 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 563E6C39
P 5350 3700
F 0 "#PWR018" H 5350 3450 50  0001 C CNN
F 1 "GND" H 5350 3550 50  0000 C CNN
F 2 "" H 5350 3700 60  0000 C CNN
F 3 "" H 5350 3700 60  0000 C CNN
	1    5350 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3500 5350 3700
NoConn ~ 5900 3100
Wire Wire Line
	2900 2900 4850 2900
Wire Wire Line
	4600 2900 4600 3100
Wire Wire Line
	4600 3100 4850 3100
Connection ~ 4600 2900
Text HLabel 6150 2900 2    60   Output ~ 0
+3V3
Wire Wire Line
	6150 2900 5900 2900
$Comp
L MCP1755 U502
U 1 1 5650E5B1
P 1850 3350
F 0 "U502" H 2500 3350 60  0000 C CNN
F 1 "MCP1755-5" H 2250 3950 60  0000 C CNN
F 2 "Housings_SOT-23_SOT-143_TSOT-6:SOT-23-5" H 1850 3350 60  0001 C CNN
F 3 "" H 1850 3350 60  0000 C CNN
	1    1850 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR019
U 1 1 5650E5B7
P 2350 3700
F 0 "#PWR019" H 2350 3450 50  0001 C CNN
F 1 "GND" H 2350 3550 50  0000 C CNN
F 2 "" H 2350 3700 60  0000 C CNN
F 3 "" H 2350 3700 60  0000 C CNN
	1    2350 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 3500 2350 3700
NoConn ~ 2900 3100
Text HLabel 1100 2900 0    60   Input ~ 0
+12V
Wire Wire Line
	1100 2900 1850 2900
Wire Wire Line
	1600 2900 1600 3100
Wire Wire Line
	1600 3100 1850 3100
Connection ~ 1600 2900
Text HLabel 3850 2600 2    60   Output ~ 0
+5V
Wire Wire Line
	3850 2600 3550 2600
Wire Wire Line
	3550 2600 3550 2900
Connection ~ 3550 2900
$Comp
L C C501
U 1 1 569058F8
P 1300 3350
F 0 "C501" H 1325 3450 50  0000 L CNN
F 1 "10μ" H 1325 3250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1338 3200 50  0001 C CNN
F 3 "" H 1300 3350 50  0000 C CNN
	1    1300 3350
	-1   0    0    1   
$EndComp
$Comp
L C C502
U 1 1 569059AC
P 1550 3350
F 0 "C502" H 1575 3450 50  0000 L CNN
F 1 ".1μ" H 1575 3250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 1588 3200 50  0001 C CNN
F 3 "" H 1550 3350 50  0000 C CNN
	1    1550 3350
	1    0    0    -1  
$EndComp
Connection ~ 1300 2900
Connection ~ 1550 2900
Wire Wire Line
	1300 2900 1300 3200
Wire Wire Line
	1550 2900 1550 3200
$Comp
L GND #PWR020
U 1 1 56905BC3
P 1300 3650
F 0 "#PWR020" H 1300 3400 50  0001 C CNN
F 1 "GND" H 1300 3500 50  0000 C CNN
F 2 "" H 1300 3650 50  0000 C CNN
F 3 "" H 1300 3650 50  0000 C CNN
	1    1300 3650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR021
U 1 1 56905BE3
P 1550 3650
F 0 "#PWR021" H 1550 3400 50  0001 C CNN
F 1 "GND" H 1550 3500 50  0000 C CNN
F 2 "" H 1550 3650 50  0000 C CNN
F 3 "" H 1550 3650 50  0000 C CNN
	1    1550 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 3500 1550 3650
Wire Wire Line
	1300 3500 1300 3650
$Comp
L C C503
U 1 1 56905D7E
P 4250 3350
F 0 "C503" H 4275 3450 50  0000 L CNN
F 1 "10μ" H 4275 3250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4288 3200 50  0001 C CNN
F 3 "" H 4250 3350 50  0000 C CNN
	1    4250 3350
	1    0    0    -1  
$EndComp
$Comp
L C C504
U 1 1 56905D84
P 4500 3350
F 0 "C504" H 4525 3450 50  0000 L CNN
F 1 ".1μ" H 4525 3250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 4538 3200 50  0001 C CNN
F 3 "" H 4500 3350 50  0000 C CNN
	1    4500 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 2900 4250 3200
Wire Wire Line
	4500 2900 4500 3200
$Comp
L GND #PWR022
U 1 1 56905D8C
P 4250 3650
F 0 "#PWR022" H 4250 3400 50  0001 C CNN
F 1 "GND" H 4250 3500 50  0000 C CNN
F 2 "" H 4250 3650 50  0000 C CNN
F 3 "" H 4250 3650 50  0000 C CNN
	1    4250 3650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 56905D92
P 4500 3650
F 0 "#PWR023" H 4500 3400 50  0001 C CNN
F 1 "GND" H 4500 3500 50  0000 C CNN
F 2 "" H 4500 3650 50  0000 C CNN
F 3 "" H 4500 3650 50  0000 C CNN
	1    4500 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 3500 4500 3650
Wire Wire Line
	4250 3500 4250 3650
Connection ~ 4500 2900
Connection ~ 4250 2900
Text HLabel 1600 1450 0    60   Input ~ 0
+12V
Wire Wire Line
	1600 1450 1800 1450
Text HLabel 3150 1450 2    60   Output ~ 0
10V
Wire Wire Line
	2600 1450 3150 1450
$Comp
L GND #PWR024
U 1 1 56918B70
P 2150 1900
F 0 "#PWR024" H 2150 1650 50  0001 C CNN
F 1 "GND" H 2150 1750 50  0000 C CNN
F 2 "" H 2150 1900 50  0000 C CNN
F 3 "" H 2150 1900 50  0000 C CNN
	1    2150 1900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR025
U 1 1 56918B99
P 2300 1900
F 0 "#PWR025" H 2300 1650 50  0001 C CNN
F 1 "GND" H 2300 1750 50  0000 C CNN
F 2 "" H 2300 1900 50  0000 C CNN
F 3 "" H 2300 1900 50  0000 C CNN
	1    2300 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 1750 2150 1900
Wire Wire Line
	2300 1750 2300 1900
$Comp
L Zsr1000gta U503
U 1 1 56918C28
P 2200 1500
F 0 "U503" H 2400 1300 50  0000 C CNN
F 1 "Zsr1000gta" H 2200 1700 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-223" H 2200 1500 50  0001 C CNN
F 3 "" H 2200 1500 50  0000 C CNN
	1    2200 1500
	1    0    0    -1  
$EndComp
$Comp
L C C505
U 1 1 56919083
P 2700 1900
F 0 "C505" H 2725 2000 50  0000 L CNN
F 1 "10μ" H 2725 1800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2738 1750 50  0001 C CNN
F 3 "" H 2700 1900 50  0000 C CNN
	1    2700 1900
	-1   0    0    1   
$EndComp
$Comp
L C C506
U 1 1 56919089
P 2950 1900
F 0 "C506" H 2975 2000 50  0000 L CNN
F 1 ".1μ" H 2975 1800 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 2988 1750 50  0001 C CNN
F 3 "" H 2950 1900 50  0000 C CNN
	1    2950 1900
	-1   0    0    1   
$EndComp
Wire Wire Line
	2700 1450 2700 1750
Wire Wire Line
	2950 1450 2950 1750
$Comp
L GND #PWR026
U 1 1 56919091
P 2700 2200
F 0 "#PWR026" H 2700 1950 50  0001 C CNN
F 1 "GND" H 2700 2050 50  0000 C CNN
F 2 "" H 2700 2200 50  0000 C CNN
F 3 "" H 2700 2200 50  0000 C CNN
	1    2700 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR027
U 1 1 56919097
P 2950 2200
F 0 "#PWR027" H 2950 1950 50  0001 C CNN
F 1 "GND" H 2950 2050 50  0000 C CNN
F 2 "" H 2950 2200 50  0000 C CNN
F 3 "" H 2950 2200 50  0000 C CNN
	1    2950 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 2050 2950 2200
Wire Wire Line
	2700 2050 2700 2200
Connection ~ 2950 1450
Connection ~ 2700 1450
$EndSCHEMATC
