EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Connector_Generic:Conn_02x23_Counter_Clockwise J1
U 1 1 5AF9BBCE
P 3275 2075
F 0 "J1" H 3325 3275 50  0000 C CNN
F 1 "Conn 1/2" V 2625 2050 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x23_Pitch2.54mm_SMD" H 3275 2075 50  0001 C CNN
F 3 "" H 3275 2075 50  0001 C CNN
	1    3275 2075
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x23_Counter_Clockwise J3
U 1 1 5AF9E625
P 3250 4575
F 0 "J3" H 3300 5775 50  0000 C CNN
F 1 "Conn 2/2" V 2600 4575 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x23_Pitch2.54mm_SMD" H 3250 4575 50  0001 C CNN
F 3 "" H 3250 4575 50  0001 C CNN
	1    3250 4575
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 4675 1675 4675
Wire Wire Line
	1450 4575 1675 4575
Wire Wire Line
	1450 4475 1675 4475
Wire Wire Line
	1450 4375 1675 4375
Wire Wire Line
	1450 4275 1675 4275
Wire Wire Line
	1450 4175 1675 4175
Wire Wire Line
	1450 3975 1675 3975
Wire Wire Line
	1450 3875 1675 3875
Wire Wire Line
	1450 3775 1675 3775
Wire Wire Line
	1450 3675 1675 3675
Wire Wire Line
	1450 3575 1675 3575
Wire Wire Line
	1450 3475 1675 3475
Wire Wire Line
	1450 4775 1675 4775
Wire Wire Line
	1450 4975 1675 4975
Wire Wire Line
	1450 5075 1675 5075
Wire Wire Line
	1450 5275 1675 5275
Wire Wire Line
	1450 5375 1675 5375
Wire Wire Line
	1450 5575 1675 5575
Wire Wire Line
	1450 3175 1675 3175
Wire Wire Line
	1450 3075 1675 3075
Wire Wire Line
	1450 2975 1675 2975
Wire Wire Line
	1450 2875 1675 2875
Wire Wire Line
	1450 2775 1675 2775
Wire Wire Line
	1450 2675 1675 2675
Wire Wire Line
	1450 2575 1675 2575
Wire Wire Line
	1450 2475 1675 2475
Wire Wire Line
	1450 2375 1675 2375
Wire Wire Line
	2825 4775 3050 4775
Wire Wire Line
	2875 2175 3075 2175
Wire Wire Line
	2875 2275 3075 2275
Wire Wire Line
	2875 2375 3075 2375
Wire Wire Line
	2875 2475 3075 2475
Wire Wire Line
	2875 2575 3075 2575
Wire Wire Line
	2875 2675 3075 2675
Wire Wire Line
	2875 2775 3075 2775
Wire Wire Line
	2875 2875 3075 2875
Wire Wire Line
	2875 2975 3075 2975
Wire Wire Line
	2875 975  3075 975 
Wire Wire Line
	2875 1275 3075 1275
Wire Wire Line
	2875 1375 3075 1375
Wire Wire Line
	2875 1475 3075 1475
Wire Wire Line
	2875 1575 3075 1575
Wire Wire Line
	2875 1675 3075 1675
Wire Wire Line
	2875 1775 3075 1775
Wire Wire Line
	2875 1875 3075 1875
Wire Wire Line
	2875 1975 3075 1975
Wire Wire Line
	3575 975  3750 975 
Wire Wire Line
	2850 3475 3050 3475
Wire Wire Line
	2850 3575 3050 3575
Wire Wire Line
	3050 3675 2850 3675
Wire Wire Line
	3050 3775 2850 3775
Wire Wire Line
	3050 3875 2850 3875
Wire Wire Line
	3050 3975 2850 3975
Wire Wire Line
	3050 4075 2850 4075
Wire Wire Line
	3050 4275 2850 4275
Wire Wire Line
	3050 4375 2850 4375
Wire Wire Line
	3050 4475 2850 4475
$Comp
L Device:LED D2
U 1 1 5AFA0424
P 2825 6475
F 0 "D2" H 2825 6575 50  0000 C CNN
F 1 "LED" H 2825 6375 50  0000 C CNN
F 2 "LED_SMD:LED_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2825 6475 50  0001 C CNN
F 3 "" H 2825 6475 50  0001 C CNN
	1    2825 6475
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5AFA0489
P 2375 6475
F 0 "R1" V 2455 6475 50  0000 C CNN
F 1 "R" V 2375 6475 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2305 6475 50  0001 C CNN
F 3 "" H 2375 6475 50  0001 C CNN
	1    2375 6475
	0    1    1    0   
$EndComp
Wire Wire Line
	3750 3175 3575 3175
Wire Wire Line
	2525 6475 2675 6475
Wire Wire Line
	3125 6475 2975 6475
Wire Wire Line
	3550 4475 3550 4375
Wire Wire Line
	3575 1975 3575 1875
Wire Wire Line
	3575 3175 3575 3075
Connection ~ 3575 3175
Connection ~ 3575 3075
Wire Wire Line
	3575 3075 3575 2975
Connection ~ 3575 2975
Wire Wire Line
	3575 2975 3575 2875
Connection ~ 3575 2875
Wire Wire Line
	3575 2875 3575 2775
Connection ~ 3575 2775
Wire Wire Line
	3575 2775 3575 2675
Connection ~ 3575 2675
Wire Wire Line
	3575 2675 3575 2575
Connection ~ 3575 2575
Wire Wire Line
	3575 2575 3575 2475
Connection ~ 3575 2475
Wire Wire Line
	3575 2475 3575 2375
Connection ~ 3575 2375
Wire Wire Line
	3575 2375 3575 2275
Connection ~ 3575 2275
Wire Wire Line
	3575 2275 3575 2175
Connection ~ 3575 1875
Wire Wire Line
	3575 1875 3575 1775
Connection ~ 3575 1775
Wire Wire Line
	3575 1775 3575 1675
Connection ~ 3575 1675
Wire Wire Line
	3575 1675 3575 1575
Connection ~ 3575 1575
Wire Wire Line
	3575 1575 3575 1475
Connection ~ 3575 1475
Wire Wire Line
	3575 1475 3575 1375
Connection ~ 3575 1375
Wire Wire Line
	3575 1375 3575 1275
Connection ~ 3575 1275
Wire Wire Line
	3575 1275 3575 1175
Connection ~ 3575 1175
Wire Wire Line
	3575 1175 3575 1075
Connection ~ 3575 1075
Wire Wire Line
	3575 1075 3575 975 
Connection ~ 3575 975 
Wire Wire Line
	3550 5675 3550 5575
Connection ~ 3550 5575
Wire Wire Line
	3550 5575 3550 5475
Connection ~ 3550 5475
Wire Wire Line
	3550 5475 3550 5375
Connection ~ 3550 5375
Wire Wire Line
	3550 5375 3550 5275
Connection ~ 3550 5275
Wire Wire Line
	3550 5275 3550 5175
Connection ~ 3550 5175
Wire Wire Line
	3550 5175 3550 5075
Connection ~ 3550 5075
Wire Wire Line
	3550 5075 3550 4975
Connection ~ 3550 4975
Wire Wire Line
	3550 4975 3550 4875
Connection ~ 3550 4875
Wire Wire Line
	3550 4875 3550 4775
Connection ~ 3550 4775
Wire Wire Line
	3550 4775 3550 4675
Connection ~ 3550 4375
Wire Wire Line
	3550 4375 3550 4275
Connection ~ 3550 4275
Wire Wire Line
	3550 4275 3550 4175
Connection ~ 3550 4175
Wire Wire Line
	3550 4175 3550 4075
Connection ~ 3550 4075
Wire Wire Line
	3550 4075 3550 3975
Connection ~ 3550 3975
Wire Wire Line
	3550 3975 3550 3875
Connection ~ 3550 3875
Wire Wire Line
	3550 3875 3550 3775
Connection ~ 3550 3775
Wire Wire Line
	3550 3775 3550 3675
Connection ~ 3550 3675
Wire Wire Line
	3550 3675 3550 3575
Connection ~ 3550 3575
Wire Wire Line
	3550 3575 3550 3475
Connection ~ 3550 3475
$Comp
L Switch:SW_Push SW1
U 1 1 5EFB2F18
P 3800 6475
F 0 "SW1" H 3800 6759 50  0000 C CNN
F 1 "SW_Push" H 3800 6668 50  0000 C CNN
F 2 "Button_Switch_SMD:Panasonic_EVQPUL_EVQPUC" H 3800 6675 50  0001 C CNN
F 3 "~" H 3800 6675 50  0001 C CNN
	1    3800 6475
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 6475 3400 6475
Wire Wire Line
	4000 6475 4150 6475
$Comp
L WSCartBreakout-rescue:Conn_01x48_Male-Connector J2
U 1 1 5AE12F15
P 1250 2875
F 0 "J2" H 1350 4900 50  0000 C CNN
F 1 "WS Conn" V 1150 2875 50  0000 C CNN
F 2 "WonderSwan:CONNECTOR" H 1250 2875 50  0001 C CNN
F 3 "" H 1250 2875 50  0001 C CNN
	1    1250 2875
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 5475 1675 5475
$Comp
L power:VSS #PWR?
U 1 1 619D2C9F
P 1800 1075
F 0 "#PWR?" H 1800 925 50  0001 C CNN
F 1 "VSS" H 1815 1248 50  0000 C CNN
F 2 "" H 1800 1075 50  0001 C CNN
F 3 "" H 1800 1075 50  0001 C CNN
	1    1800 1075
	-1   0    0    1   
$EndComp
$Comp
L power:VSS #PWR?
U 1 1 619D40D9
P 2050 6475
F 0 "#PWR?" H 2050 6325 50  0001 C CNN
F 1 "VSS" H 2065 6648 50  0000 C CNN
F 2 "" H 2050 6475 50  0001 C CNN
F 3 "" H 2050 6475 50  0001 C CNN
	1    2050 6475
	-1   0    0    1   
$EndComp
Wire Wire Line
	2050 6475 2225 6475
$Comp
L power:VDD #PWR?
U 1 1 61A2DA60
P 3125 6475
F 0 "#PWR?" H 3125 6325 50  0001 C CNN
F 1 "VDD" H 3140 6648 50  0000 C CNN
F 2 "" H 3125 6475 50  0001 C CNN
F 3 "" H 3125 6475 50  0001 C CNN
	1    3125 6475
	1    0    0    -1  
$EndComp
$Comp
L power:VSS #PWR?
U 1 1 61AB44A0
P 4150 6475
F 0 "#PWR?" H 4150 6325 50  0001 C CNN
F 1 "VSS" H 4165 6648 50  0000 C CNN
F 2 "" H 4150 6475 50  0001 C CNN
F 3 "" H 4150 6475 50  0001 C CNN
	1    4150 6475
	-1   0    0    1   
$EndComp
Text Label 3400 6475 0    50   ~ 0
~RST
Text Label 1675 4875 2    50   ~ 0
~RST
Wire Wire Line
	1450 4875 1675 4875
Text Label 2850 4175 0    50   ~ 0
~RST
Wire Wire Line
	2850 4175 3050 4175
Wire Wire Line
	1450 5675 1800 5675
$Comp
L power:VSS #PWR?
U 1 1 61C19E27
P 1800 5675
F 0 "#PWR?" H 1800 5525 50  0001 C CNN
F 1 "VSS" H 1815 5848 50  0000 C CNN
F 2 "" H 1800 5675 50  0001 C CNN
F 3 "" H 1800 5675 50  0001 C CNN
	1    1800 5675
	-1   0    0    1   
$EndComp
$Comp
L power:VSS #PWR?
U 1 1 61C1DDBE
P 3750 975
F 0 "#PWR?" H 3750 825 50  0001 C CNN
F 1 "VSS" H 3765 1148 50  0000 C CNN
F 2 "" H 3750 975 50  0001 C CNN
F 3 "" H 3750 975 50  0001 C CNN
	1    3750 975 
	-1   0    0    1   
$EndComp
$Comp
L power:VSS #PWR?
U 1 1 61C1F3B0
P 3750 3175
F 0 "#PWR?" H 3750 3025 50  0001 C CNN
F 1 "VSS" H 3765 3348 50  0000 C CNN
F 2 "" H 3750 3175 50  0001 C CNN
F 3 "" H 3750 3175 50  0001 C CNN
	1    3750 3175
	-1   0    0    1   
$EndComp
$Comp
L power:VSS #PWR?
U 1 1 61C2070D
P 3750 5675
F 0 "#PWR?" H 3750 5525 50  0001 C CNN
F 1 "VSS" H 3765 5848 50  0000 C CNN
F 2 "" H 3750 5675 50  0001 C CNN
F 3 "" H 3750 5675 50  0001 C CNN
	1    3750 5675
	-1   0    0    1   
$EndComp
$Comp
L power:VSS #PWR?
U 1 1 61C20F62
P 3750 3475
F 0 "#PWR?" H 3750 3325 50  0001 C CNN
F 1 "VSS" H 3765 3648 50  0000 C CNN
F 2 "" H 3750 3475 50  0001 C CNN
F 3 "" H 3750 3475 50  0001 C CNN
	1    3750 3475
	-1   0    0    1   
$EndComp
Wire Wire Line
	3550 3475 3750 3475
Wire Wire Line
	3550 5675 3750 5675
Connection ~ 3550 5675
Text Label 1675 1075 2    50   ~ 0
A14
Wire Wire Line
	1450 1075 1675 1075
Text Label 2825 5675 0    50   ~ 0
A14
Wire Wire Line
	3050 5675 2825 5675
Text Label 1675 1175 2    50   ~ 0
A9
Wire Wire Line
	1450 1175 1675 1175
Text Label 2825 5575 0    50   ~ 0
A9
Wire Wire Line
	3050 5575 2825 5575
Text Label 1675 1275 2    50   ~ 0
A10
Wire Wire Line
	1450 1275 1675 1275
Text Label 2825 5475 0    50   ~ 0
A10
Wire Wire Line
	3050 5475 2825 5475
Text Label 1675 1375 2    50   ~ 0
A8
Wire Wire Line
	1450 1375 1675 1375
Text Label 1675 1475 2    50   ~ 0
A7
Wire Wire Line
	1450 1475 1675 1475
Text Label 2825 5375 0    50   ~ 0
A8
Wire Wire Line
	3050 5375 2825 5375
Text Label 2825 5275 0    50   ~ 0
A7
Wire Wire Line
	3050 5275 2825 5275
Text Label 1675 1575 2    50   ~ 0
A12
Wire Wire Line
	1450 1575 1675 1575
Text Label 2825 5175 0    50   ~ 0
A12
Wire Wire Line
	3050 5175 2825 5175
Text Label 1675 1675 2    50   ~ 0
A13
Wire Wire Line
	1450 1675 1675 1675
Text Label 2825 5075 0    50   ~ 0
A13
Wire Wire Line
	2825 5075 3050 5075
Text Label 1675 1775 2    50   ~ 0
A11
Wire Wire Line
	1450 1775 1675 1775
Text Label 2825 4975 0    50   ~ 0
A11
Wire Wire Line
	3050 4975 2825 4975
Text Label 1675 1875 2    50   ~ 0
A6
Wire Wire Line
	1450 1875 1675 1875
Text Label 2825 4875 0    50   ~ 0
A6
Wire Wire Line
	2825 4875 3050 4875
Text Label 1675 1975 2    50   ~ 0
A5
Wire Wire Line
	1450 1975 1675 1975
Text Label 2825 4775 0    50   ~ 0
A5
Text Label 1675 2075 2    50   ~ 0
A4
Wire Wire Line
	1450 2075 1675 2075
Wire Wire Line
	2875 3075 3075 3075
Wire Wire Line
	1450 2275 1675 2275
Wire Wire Line
	2875 3175 3075 3175
Wire Wire Line
	3075 1075 2875 1075
Text Label 2825 4675 0    50   ~ 0
A4
Wire Wire Line
	3050 4675 2825 4675
Text Label 1675 2175 2    50   ~ 0
A3
Wire Wire Line
	1450 2175 1675 2175
Text Label 2875 3175 0    50   ~ 0
A3
Text Label 1675 2275 2    50   ~ 0
Q15
Text Label 2875 3075 0    50   ~ 0
Q15
Text Label 1675 2375 2    50   ~ 0
Q14
Text Label 2875 2975 0    50   ~ 0
Q14
Text Label 1675 2475 2    50   ~ 0
Q7
Text Label 2875 2875 0    50   ~ 0
Q7
Text Label 1675 2575 2    50   ~ 0
Q6
Text Label 2875 2775 0    50   ~ 0
Q6
Text Label 1675 2675 2    50   ~ 0
Q5
Text Label 2875 2675 0    50   ~ 0
Q5
Text Label 1675 2775 2    50   ~ 0
Q4
Text Label 2875 2575 0    50   ~ 0
Q4
Text Label 1675 2875 2    50   ~ 0
Q3
Text Label 2875 2475 0    50   ~ 0
Q3
Text Label 1675 2975 2    50   ~ 0
Q2
Text Label 2875 2375 0    50   ~ 0
Q2
Text Label 1675 3075 2    50   ~ 0
Q1
Text Label 2875 2275 0    50   ~ 0
Q1
Text Label 1675 3175 2    50   ~ 0
Q0
$Comp
L power:VDD #PWR?
U 1 1 62491319
P 1800 3175
F 0 "#PWR?" H 1800 3025 50  0001 C CNN
F 1 "VDD" H 1815 3348 50  0000 C CNN
F 2 "" H 1800 3175 50  0001 C CNN
F 3 "" H 1800 3175 50  0001 C CNN
	1    1800 3175
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 3275 1800 3275
Wire Wire Line
	1450 3375 1800 3375
Wire Wire Line
	1800 3375 1800 3275
Connection ~ 1800 3275
Wire Wire Line
	1800 3175 1800 3275
Wire Wire Line
	1450 975  1800 975 
Wire Wire Line
	1800 975  1800 1075
$Comp
L power:VDD #PWR?
U 1 1 6251A934
P 2775 2175
F 0 "#PWR?" H 2775 2025 50  0001 C CNN
F 1 "VDD" H 2700 2325 50  0000 L CNN
F 2 "" H 2775 2175 50  0001 C CNN
F 3 "" H 2775 2175 50  0001 C CNN
	1    2775 2175
	-1   0    0    1   
$EndComp
$Comp
L power:VDD #PWR?
U 1 1 62530E6F
P 3750 2075
F 0 "#PWR?" H 3750 1925 50  0001 C CNN
F 1 "VDD" H 3675 2225 50  0000 L CNN
F 2 "" H 3750 2075 50  0001 C CNN
F 3 "" H 3750 2075 50  0001 C CNN
	1    3750 2075
	-1   0    0    1   
$EndComp
Wire Wire Line
	3750 2075 3575 2075
Text Label 1675 3475 2    50   ~ 0
A-1
Text Label 2875 1975 0    50   ~ 0
A-1
Text Label 2875 2175 0    50   ~ 0
Q0
Wire Wire Line
	2775 2175 2775 2075
Wire Wire Line
	2775 2075 3075 2075
Text Label 1675 3575 2    50   ~ 0
A0
Text Label 2875 1875 0    50   ~ 0
A0
Text Label 1675 3675 2    50   ~ 0
A1
Text Label 2875 1775 0    50   ~ 0
A1
Text Label 1675 3775 2    50   ~ 0
A2
Text Label 2875 1675 0    50   ~ 0
A2
Text Label 1675 3875 2    50   ~ 0
A18
Text Label 2875 1575 0    50   ~ 0
A18
Text Label 1675 3975 2    50   ~ 0
A17
Text Label 2875 1475 0    50   ~ 0
A17
Text Label 1675 4075 2    50   ~ 0
A16
Wire Wire Line
	1450 4075 1675 4075
Text Label 2875 1375 0    50   ~ 0
A16
Text Label 1675 5575 2    50   ~ 0
CLK
Text Label 2850 3475 0    50   ~ 0
CLK
Text Label 2850 3575 0    50   ~ 0
~INT
Text Label 1675 5475 2    50   ~ 0
~INT
Text Label 2850 3675 0    50   ~ 0
~CS
Text Label 1675 5375 2    50   ~ 0
~CS
Text Label 2850 3775 0    50   ~ 0
~WE
Text Label 1675 5275 2    50   ~ 0
~WE
Text Label 2850 3875 0    50   ~ 0
~OE
Wire Wire Line
	1450 5175 1675 5175
Text Label 1675 5175 2    50   ~ 0
~OE
Text Label 2850 3975 0    50   ~ 0
~SS
Text Label 2850 4075 0    50   ~ 0
~SO
Text Label 1675 5075 2    50   ~ 0
~SS
Text Label 1675 4975 2    50   ~ 0
~SO
Text Label 2850 4275 0    50   ~ 0
Q13
Text Label 1675 4775 2    50   ~ 0
Q13
Text Label 2850 4375 0    50   ~ 0
Q12
Text Label 1675 4675 2    50   ~ 0
Q12
Text Label 2850 4475 0    50   ~ 0
Q11
Text Label 1675 4575 2    50   ~ 0
Q11
$Comp
L power:VDD #PWR?
U 1 1 6281C538
P 2725 4675
F 0 "#PWR?" H 2725 4525 50  0001 C CNN
F 1 "VDD" H 2650 4825 50  0000 L CNN
F 2 "" H 2725 4675 50  0001 C CNN
F 3 "" H 2725 4675 50  0001 C CNN
	1    2725 4675
	-1   0    0    1   
$EndComp
Wire Wire Line
	3050 4575 2725 4575
Wire Wire Line
	2725 4575 2725 4675
Text Label 2875 975  0    50   ~ 0
Q10
Text Label 2875 1075 0    50   ~ 0
Q9
Text Label 2875 1175 0    50   ~ 0
Q8
Text Label 2875 1275 0    50   ~ 0
A15
Wire Wire Line
	2875 1175 3075 1175
Text Label 1675 4475 2    50   ~ 0
Q10
Text Label 1675 4375 2    50   ~ 0
Q9
Text Label 1675 4275 2    50   ~ 0
Q8
Text Label 1675 4175 2    50   ~ 0
A15
$Comp
L WonderSwan:EDGE U4
U 1 1 5A359571
P 5400 3325
F 0 "U4" V 2825 3350 60  0000 C CNN
F 1 "WS Edge" H 5400 3600 60  0000 C CNN
F 2 "WonderSwan:EDGE" H 3850 3875 60  0001 C CNN
F 3 "" H 3850 3875 60  0001 C CNN
	1    5400 3325
	0    1    1    0   
$EndComp
Text Label 4875 4175 0    50   ~ 0
A15
Text Label 4875 4275 0    50   ~ 0
Q8
Text Label 4875 4375 0    50   ~ 0
Q9
Text Label 4875 4475 0    50   ~ 0
Q10
Text Label 4875 4575 0    50   ~ 0
Q11
Text Label 4875 4675 0    50   ~ 0
Q12
Text Label 4875 4775 0    50   ~ 0
Q13
Text Label 4875 4975 0    50   ~ 0
~SO
Text Label 4875 5075 0    50   ~ 0
~SS
Text Label 4875 5175 0    50   ~ 0
~OE
Text Label 4875 5275 0    50   ~ 0
~WE
Text Label 4875 5375 0    50   ~ 0
~CS
Text Label 4875 5475 0    50   ~ 0
~INT
Text Label 4875 5575 0    50   ~ 0
CLK
Text Label 4875 4075 0    50   ~ 0
A16
Text Label 4875 3975 0    50   ~ 0
A17
Text Label 4875 3875 0    50   ~ 0
A18
Text Label 4875 3775 0    50   ~ 0
A2
Text Label 4875 3675 0    50   ~ 0
A1
Text Label 4875 3575 0    50   ~ 0
A0
Text Label 4875 3475 0    50   ~ 0
A-1
Text Label 4875 3175 0    50   ~ 0
Q0
Text Label 4875 3075 0    50   ~ 0
Q1
Text Label 4875 2975 0    50   ~ 0
Q2
Text Label 4875 2875 0    50   ~ 0
Q3
Text Label 4875 2775 0    50   ~ 0
Q4
Text Label 4875 2675 0    50   ~ 0
Q5
Text Label 4875 2575 0    50   ~ 0
Q6
Text Label 4875 2475 0    50   ~ 0
Q7
Text Label 4875 2375 0    50   ~ 0
Q14
Text Label 4875 2275 0    50   ~ 0
Q15
Text Label 4875 2175 0    50   ~ 0
A3
Text Label 4875 2075 0    50   ~ 0
A4
Text Label 4875 1975 0    50   ~ 0
A5
Text Label 4875 1875 0    50   ~ 0
A6
Text Label 4875 1775 0    50   ~ 0
A11
Text Label 4875 1675 0    50   ~ 0
A13
Text Label 4875 1575 0    50   ~ 0
A12
Text Label 4875 1475 0    50   ~ 0
A7
Text Label 4875 1375 0    50   ~ 0
A8
Text Label 4875 1275 0    50   ~ 0
A10
Text Label 4875 1175 0    50   ~ 0
A9
Text Label 4875 4875 0    50   ~ 0
~RST
Text Label 4875 1075 0    50   ~ 0
A14
$Comp
L power:VDD #PWR?
U 1 1 629F9DEB
P 3750 4575
F 0 "#PWR?" H 3750 4425 50  0001 C CNN
F 1 "VDD" H 3675 4725 50  0000 L CNN
F 2 "" H 3750 4575 50  0001 C CNN
F 3 "" H 3750 4575 50  0001 C CNN
	1    3750 4575
	-1   0    0    1   
$EndComp
Wire Wire Line
	3550 4575 3750 4575
Wire Wire Line
	4875 1075 5100 1075
Wire Wire Line
	4875 1175 5100 1175
Wire Wire Line
	5100 1275 4875 1275
Wire Wire Line
	4875 1375 5100 1375
Wire Wire Line
	5100 1475 4875 1475
Wire Wire Line
	4875 1575 5100 1575
Wire Wire Line
	5100 1675 4875 1675
Wire Wire Line
	4875 1775 5100 1775
Wire Wire Line
	5100 1875 4875 1875
Wire Wire Line
	4875 1975 5100 1975
Wire Wire Line
	5100 2075 4875 2075
Wire Wire Line
	4875 2175 5100 2175
Wire Wire Line
	4875 2275 5100 2275
Wire Wire Line
	4875 2375 5100 2375
Wire Wire Line
	4875 2475 5100 2475
Wire Wire Line
	4875 2575 5100 2575
Wire Wire Line
	4875 2675 5100 2675
Wire Wire Line
	4875 2775 5100 2775
Wire Wire Line
	4875 2875 5100 2875
Wire Wire Line
	4875 2975 5100 2975
Wire Wire Line
	4875 3075 5100 3075
Wire Wire Line
	4875 3175 5100 3175
Wire Wire Line
	4875 3475 5100 3475
Wire Wire Line
	4875 3575 5100 3575
Wire Wire Line
	4875 3675 5100 3675
Wire Wire Line
	4875 3875 5100 3875
Wire Wire Line
	4875 3975 5100 3975
Wire Wire Line
	4875 4075 5100 4075
Wire Wire Line
	4875 4175 5100 4175
Wire Wire Line
	4875 4275 5100 4275
Wire Wire Line
	4875 4375 5100 4375
Wire Wire Line
	4875 4475 5100 4475
Wire Wire Line
	5100 4575 4875 4575
Wire Wire Line
	4875 4675 5100 4675
Wire Wire Line
	5100 4775 4875 4775
Wire Wire Line
	4875 4875 5100 4875
Wire Wire Line
	4875 4975 5100 4975
Wire Wire Line
	4875 5075 5100 5075
Wire Wire Line
	4875 5175 5100 5175
Wire Wire Line
	4875 5275 5100 5275
Wire Wire Line
	4875 5375 5100 5375
Wire Wire Line
	4875 5475 5100 5475
Wire Wire Line
	4875 5575 5100 5575
$Comp
L power:VSS #PWR?
U 1 1 62C13E79
P 4750 5675
F 0 "#PWR?" H 4750 5525 50  0001 C CNN
F 1 "VSS" H 4765 5848 50  0000 C CNN
F 2 "" H 4750 5675 50  0001 C CNN
F 3 "" H 4750 5675 50  0001 C CNN
	1    4750 5675
	-1   0    0    1   
$EndComp
Wire Wire Line
	5100 5675 4750 5675
$Comp
L power:VDD #PWR?
U 1 1 62C33F03
P 4700 3175
F 0 "#PWR?" H 4700 3025 50  0001 C CNN
F 1 "VDD" H 4715 3348 50  0000 C CNN
F 2 "" H 4700 3175 50  0001 C CNN
F 3 "" H 4700 3175 50  0001 C CNN
	1    4700 3175
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 3175 4700 3275
Wire Wire Line
	4700 3275 5100 3275
Wire Wire Line
	4700 3375 4700 3275
Wire Wire Line
	4700 3375 5100 3375
Connection ~ 4700 3275
$Comp
L power:VSS #PWR?
U 1 1 62C50EDE
P 4700 1075
F 0 "#PWR?" H 4700 925 50  0001 C CNN
F 1 "VSS" H 4715 1248 50  0000 C CNN
F 2 "" H 4700 1075 50  0001 C CNN
F 3 "" H 4700 1075 50  0001 C CNN
	1    4700 1075
	-1   0    0    1   
$EndComp
Wire Wire Line
	5100 975  4700 975 
Wire Wire Line
	4700 975  4700 1075
$EndSCHEMATC
