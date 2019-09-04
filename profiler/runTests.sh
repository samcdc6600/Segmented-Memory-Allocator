#!/bin/sh
echo "Starting tests that don't call printStats()\n"
# Test 0
#	Policy 0
./profiler 0 0 0 0 > testresults/0/noStats/0000 &
./profiler 0 0 1 0 > testresults/0/noStats/0010 &
./profiler 0 0 2 0 > testresults/0/noStats/0020 &
./profiler 0 0 3 0 > testresults/0/noStats/0030 &
./profiler 0 0 4 0 > testresults/0/noStats/0040 &
./profiler 0 0 5 0 > testresults/0/noStats/0050 &
./profiler 0 0 6 0 > testresults/0/noStats/0060 &
#	Policy 1
./profiler 1 0 0 0 > testresults/0/noStats/1000 &
./profiler 1 0 1 0 > testresults/0/noStats/1010 &
./profiler 1 0 2 0 > testresults/0/noStats/1020 &
./profiler 1 0 3 0 > testresults/0/noStats/1030 &
./profiler 1 0 4 0 > testresults/0/noStats/1040 &
./profiler 1 0 5 0 > testresults/0/noStats/1050 &
./profiler 1 0 6 0 > testresults/0/noStats/1060 &
#	Policy 2
./profiler 2 0 0 0 > testresults/0/noStats/2000 &
./profiler 2 0 1 0 > testresults/0/noStats/2010 &
./profiler 2 0 2 0 > testresults/0/noStats/2020 &
./profiler 2 0 3 0 > testresults/0/noStats/2030 &
./profiler 2 0 4 0 > testresults/0/noStats/2040 &
./profiler 2 0 5 0 > testresults/0/noStats/2050 &
./profiler 2 0 6 0 > testresults/0/noStats/2060 &

# Test 1
#	Policy 0
./profiler 0 1 0 0 > testresults/1/noStats/0100 &
./profiler 0 1 1 0 > testresults/1/noStats/0110 &
./profiler 0 1 2 0 > testresults/1/noStats/0120 &
./profiler 0 1 3 0 > testresults/1/noStats/0130 &
./profiler 0 1 4 0 > testresults/1/noStats/0140 &
./profiler 0 1 5 0 > testresults/1/noStats/0150 &
./profiler 0 1 6 0 > testresults/1/noStats/0160 &
#	Policy 1
./profiler 1 1 0 0 > testresults/1/noStats/1100 &
./profiler 1 1 1 0 > testresults/1/noStats/1110 &
./profiler 1 1 2 0 > testresults/1/noStats/1120 &
./profiler 1 1 3 0 > testresults/1/noStats/1130 &
./profiler 1 1 4 0 > testresults/1/noStats/1140 &
./profiler 1 1 5 0 > testresults/1/noStats/1150 &
./profiler 1 1 6 0 > testresults/1/noStats/1160 &
#	Policy 2
./profiler 2 1 0 0 > testresults/1/noStats/2100 &
./profiler 2 1 1 0 > testresults/1/noStats/2110 &
./profiler 2 1 2 0 > testresults/1/noStats/2120 &
./profiler 2 1 3 0 > testresults/1/noStats/2130 &
./profiler 2 1 4 0 > testresults/1/noStats/2140 &
./profiler 2 1 5 0 > testresults/1/noStats/2150 &
./profiler 2 1 6 0 > testresults/1/noStats/2160 &

# Test 2
#	Policy 0
./profiler 0 2 0 0 > testresults/2/noStats/0200 &
./profiler 0 2 1 0 > testresults/2/noStats/0210 &
./profiler 0 2 2 0 > testresults/2/noStats/0220 &
./profiler 0 2 3 0 > testresults/2/noStats/0230 &
./profiler 0 2 4 0 > testresults/2/noStats/0240 &
./profiler 0 2 5 0 > testresults/2/noStats/0250 &
./profiler 0 2 6 0 > testresults/2/noStats/0260 &
#	Policy 1
./profiler 1 2 0 0 > testresults/2/noStats/1200 &
./profiler 1 2 1 0 > testresults/2/noStats/1210 &
./profiler 1 2 2 0 > testresults/2/noStats/1220 &
./profiler 1 2 3 0 > testresults/2/noStats/1230 &
./profiler 1 2 4 0 > testresults/2/noStats/1240 &
./profiler 1 2 5 0 > testresults/2/noStats/1250 &
./profiler 1 2 6 0 > testresults/2/noStats/1260 &
#	Policy 2
./profiler 2 2 0 0 > testresults/2/noStats/2200 &
./profiler 2 2 1 0 > testresults/2/noStats/2210 &
./profiler 2 2 2 0 > testresults/2/noStats/2220 &
./profiler 2 2 3 0 > testresults/2/noStats/2230 &
./profiler 2 2 4 0 > testresults/2/noStats/2240 &
./profiler 2 2 5 0 > testresults/2/noStats/2250 &
./profiler 2 2 6 0 > testresults/2/noStats/2260 &

# Test 3
#	Policy 0
./profiler 0 3 0 0 > testresults/3/noStats/0300 &
./profiler 0 3 1 0 > testresults/3/noStats/0310 &
./profiler 0 3 2 0 > testresults/3/noStats/0320 &
./profiler 0 3 3 0 > testresults/3/noStats/0330 &
./profiler 0 3 4 0 > testresults/3/noStats/0340 &
./profiler 0 3 5 0 > testresults/3/noStats/0350 &
./profiler 0 3 6 0 > testresults/3/noStats/0360 &
#	Policy 1
./profiler 1 3 0 0 > testresults/3/noStats/1300 &
./profiler 1 3 1 0 > testresults/3/noStats/1310 &
./profiler 1 3 2 0 > testresults/3/noStats/1320 &
./profiler 1 3 3 0 > testresults/3/noStats/1330 &
./profiler 1 3 4 0 > testresults/3/noStats/1340 &
./profiler 1 3 5 0 > testresults/3/noStats/1350 &
./profiler 1 3 6 0 > testresults/3/noStats/1360 &
#	Policy 2
./profiler 2 3 0 0 > testresults/3/noStats/2300 &
./profiler 2 3 1 0 > testresults/3/noStats/2310 &
./profiler 2 3 2 0 > testresults/3/noStats/2320 &
./profiler 2 3 3 0 > testresults/3/noStats/2330 &
./profiler 2 3 4 0 > testresults/3/noStats/2340 &
./profiler 2 3 5 0 > testresults/3/noStats/2350 &
./profiler 2 3 6 0 > testresults/3/noStats/2360 &

# Test 4
#	Policy 0
./profiler 0 4 0 0 > testresults/4/noStats/0400 &
./profiler 0 4 1 0 > testresults/4/noStats/0410 &
./profiler 0 4 2 0 > testresults/4/noStats/0420 &
./profiler 0 4 3 0 > testresults/4/noStats/0430 &
./profiler 0 4 4 0 > testresults/4/noStats/0440 &
./profiler 0 4 5 0 > testresults/4/noStats/0450 &
./profiler 0 4 6 0 > testresults/4/noStats/0460 &
#	Policy 1
./profiler 1 4 0 0 > testresults/4/noStats/1400 &
./profiler 1 4 1 0 > testresults/4/noStats/1410 &
./profiler 1 4 2 0 > testresults/4/noStats/1420 &
./profiler 1 4 3 0 > testresults/4/noStats/1430 &
./profiler 1 4 4 0 > testresults/4/noStats/1440 &
./profiler 1 4 5 0 > testresults/4/noStats/1450 &
./profiler 1 4 6 0 > testresults/4/noStats/1460 &
#	Policy 2
./profiler 2 4 0 0 > testresults/4/noStats/2400 &
./profiler 2 4 1 0 > testresults/4/noStats/2410 &
./profiler 2 4 2 0 > testresults/4/noStats/2420 &
./profiler 2 4 3 0 > testresults/4/noStats/2430 &
./profiler 2 4 4 0 > testresults/4/noStats/2440 &
./profiler 2 4 5 0 > testresults/4/noStats/2450 &
./profiler 2 4 6 0 > testresults/4/noStats/2460 &

# Test 5
#	Policy 0
./profiler 0 5 0 0 > testresults/5/noStats/0500 &
./profiler 0 5 1 0 > testresults/5/noStats/0510 &
./profiler 0 5 2 0 > testresults/5/noStats/0520 &
./profiler 0 5 3 0 > testresults/5/noStats/0530 &
./profiler 0 5 4 0 > testresults/5/noStats/0540 &
./profiler 0 5 5 0 > testresults/5/noStats/0550 &
./profiler 0 5 6 0 > testresults/5/noStats/0560 &
#	Policy 1
./profiler 1 5 0 0 > testresults/5/noStats/1500 &
./profiler 1 5 1 0 > testresults/5/noStats/1510 &
./profiler 1 5 2 0 > testresults/5/noStats/1520 &
./profiler 1 5 3 0 > testresults/5/noStats/1530 &
./profiler 1 5 4 0 > testresults/5/noStats/1540 &
./profiler 1 5 5 0 > testresults/5/noStats/1550 &
./profiler 1 5 6 0 > testresults/5/noStats/1560 &
#	Policy 2
./profiler 2 5 0 0 > testresults/5/noStats/2500 &
./profiler 2 5 1 0 > testresults/5/noStats/2510 &
./profiler 2 5 2 0 > testresults/5/noStats/2520 &
./profiler 2 5 3 0 > testresults/5/noStats/2530 &
./profiler 2 5 4 0 > testresults/5/noStats/2540 &
./profiler 2 5 5 0 > testresults/5/noStats/2550 &
./profiler 2 5 6 0 > testresults/5/noStats/2560 &

# Test 6
#	Policy 0
./profiler 0 6 0 0 > testresults/6/noStats/0600 &
./profiler 0 6 1 0 > testresults/6/noStats/0610 &
./profiler 0 6 2 0 > testresults/6/noStats/0620 &
./profiler 0 6 3 0 > testresults/6/noStats/0630 &
./profiler 0 6 4 0 > testresults/6/noStats/0640 &
./profiler 0 6 5 0 > testresults/6/noStats/0650 &
./profiler 0 6 6 0 > testresults/6/noStats/0660 &
#	Policy 1
./profiler 1 6 0 0 > testresults/6/noStats/1600 &
./profiler 1 6 1 0 > testresults/6/noStats/1610 &
./profiler 1 6 2 0 > testresults/6/noStats/1620 &
./profiler 1 6 3 0 > testresults/6/noStats/1630 &
./profiler 1 6 4 0 > testresults/6/noStats/1640 &
./profiler 1 6 5 0 > testresults/6/noStats/1650 &
./profiler 1 6 6 0 > testresults/6/noStats/1660 &
#	Policy 2 
./profiler 2 6 0 0 > testresults/6/noStats/2600 &
./profiler 2 6 1 0 > testresults/6/noStats/2610 &
./profiler 2 6 2 0 > testresults/6/noStats/2620 &
./profiler 2 6 3 0 > testresults/6/noStats/2630 &
./profiler 2 6 4 0 > testresults/6/noStats/2640 &
./profiler 2 6 5 0 > testresults/6/noStats/2650 &
./profiler 2 6 6 0 > testresults/6/noStats/2660 &

# Test 7
#	Policy 0
./profiler 0 7 0 0 > testresults/7/noStats/0500 &
./profiler 0 7 1 0 > testresults/7/noStats/0510 &
./profiler 0 7 2 0 > testresults/7/noStats/0520 &
./profiler 0 7 3 0 > testresults/7/noStats/0530 &
./profiler 0 7 4 0 > testresults/7/noStats/0540 &
./profiler 0 7 5 0 > testresults/7/noStats/0550 &
./profiler 0 7 6 0 > testresults/7/noStats/0560 &
#	Policy 1 
./profiler 1 7 0 0 > testresults/7/noStats/1700 &
./profiler 1 7 1 0 > testresults/7/noStats/1710 &
./profiler 1 7 2 0 > testresults/7/noStats/1720 &
./profiler 1 7 3 0 > testresults/7/noStats/1730 &
./profiler 1 7 4 0 > testresults/7/noStats/1740 &
./profiler 1 7 5 0 > testresults/7/noStats/1750 &
./profiler 1 7 6 0 > testresults/7/noStats/1760 &
#	Policy 2
./profiler 2 7 0 0 > testresults/7/noStats/2700 &
./profiler 2 7 1 0 > testresults/7/noStats/2710 &
./profiler 2 7 2 0 > testresults/7/noStats/2720 &
./profiler 2 7 3 0 > testresults/7/noStats/2730 &
./profiler 2 7 4 0 > testresults/7/noStats/2740 &
./profiler 2 7 5 0 > testresults/7/noStats/2750 &
./profiler 2 7 6 0 > testresults/7/noStats/2760 &
#=========BTW Everything is relative to the width of my monitor roughly divided by three. Not 70 coloumns=============
echo "Starting tests that call printStarts()\n"
# Tests with additonal printStats() output
# Test 0
#	Policy 0
./profiler 0 0 0 1 > testresults/0/noStats/0001 &
./profiler 0 0 1 1 > testresults/0/noStats/0011 &
./profiler 0 0 2 1 > testresults/0/noStats/0021 &
./profiler 0 0 3 1 > testresults/0/noStats/0031 &
./profiler 0 0 4 1 > testresults/0/noStats/0041 &
./profiler 0 0 5 1 > testresults/0/noStats/0051 &
./profiler 0 0 6 1 > testresults/0/noStats/0061 &
#	Policy 1
./profiler 1 0 0 1 > testresults/0/noStats/1001 &
./profiler 1 0 1 1 > testresults/0/noStats/1011 &
./profiler 1 0 2 1 > testresults/0/noStats/1021 &
./profiler 1 0 3 1 > testresults/0/noStats/1031 &
./profiler 1 0 4 1 > testresults/0/noStats/1041 &
./profiler 1 0 5 1 > testresults/0/noStats/1051 &
./profiler 1 0 6 1 > testresults/0/noStats/1061 &
#	Policy 2
./profiler 2 0 0 1 > testresults/0/noStats/2001 &
./profiler 2 0 1 1 > testresults/0/noStats/2011 &
./profiler 2 0 2 1 > testresults/0/noStats/2021 &
./profiler 2 0 3 1 > testresults/0/noStats/2031 &
./profiler 2 0 4 1 > testresults/0/noStats/2041 &
./profiler 2 0 5 1 > testresults/0/noStats/2051 &
./profiler 2 0 6 1 > testresults/0/noStats/2061 &

# Test 1
#	Policy 0
./profiler 0 1 0 1 > testresults/1/noStats/0101 &
./profiler 0 1 1 1 > testresults/1/noStats/0111 &
./profiler 0 1 2 1 > testresults/1/noStats/0121 &
./profiler 0 1 3 1 > testresults/1/noStats/0131 &
./profiler 0 1 4 1 > testresults/1/noStats/0141 &
./profiler 0 1 5 1 > testresults/1/noStats/0151 &
./profiler 0 1 6 1 > testresults/1/noStats/0161 &
#	Policy 1
./profiler 1 1 0 1 > testresults/1/noStats/1101 &
./profiler 1 1 1 1 > testresults/1/noStats/1111 &
./profiler 1 1 2 1 > testresults/1/noStats/1121 &
./profiler 1 1 3 1 > testresults/1/noStats/1131 &
./profiler 1 1 4 1 > testresults/1/noStats/1141 &
./profiler 1 1 5 1 > testresults/1/noStats/1151 &
./profiler 1 1 6 1 > testresults/1/noStats/1161 &
#	Policy 2
./profiler 2 1 0 1 > testresults/1/noStats/2101 &
./profiler 2 1 1 1 > testresults/1/noStats/2111 &
./profiler 2 1 2 1 > testresults/1/noStats/2121 &
./profiler 2 1 3 1 > testresults/1/noStats/2131 &
./profiler 2 1 4 1 > testresults/1/noStats/2141 &
./profiler 2 1 5 1 > testresults/1/noStats/2151 &
./profiler 2 1 6 1 > testresults/1/noStats/2161 &

# Test 2
#	Policy 0
./profiler 0 2 0 1 > testresults/2/noStats/0201 &
./profiler 0 2 1 1 > testresults/2/noStats/0211 &
./profiler 0 2 2 1 > testresults/2/noStats/0221 &
./profiler 0 2 3 1 > testresults/2/noStats/0231 &
./profiler 0 2 4 1 > testresults/2/noStats/0241 &
./profiler 0 2 5 1 > testresults/2/noStats/0251 &
./profiler 0 2 6 1 > testresults/2/noStats/0261 &
#	Policy 1
./profiler 1 2 0 1 > testresults/2/noStats/1201 &
./profiler 1 2 1 1 > testresults/2/noStats/1211 &
./profiler 1 2 2 1 > testresults/2/noStats/1221 &
./profiler 1 2 3 1 > testresults/2/noStats/1231 &
./profiler 1 2 4 1 > testresults/2/noStats/1241 &
./profiler 1 2 5 1 > testresults/2/noStats/1251 &
./profiler 1 2 6 1 > testresults/2/noStats/1261 &
#	Policy 2
./profiler 2 2 0 1 > testresults/2/noStats/2201 &
./profiler 2 2 1 1 > testresults/2/noStats/2211 &
./profiler 2 2 2 1 > testresults/2/noStats/2221 &
./profiler 2 2 3 1 > testresults/2/noStats/2231 &
./profiler 2 2 4 1 > testresults/2/noStats/2241 &
./profiler 2 2 5 1 > testresults/2/noStats/2251 &
./profiler 2 2 6 1 > testresults/2/noStats/2261 &

# Test 3
#	Policy 0
./profiler 0 3 0 1 > testresults/3/noStats/0301 &
./profiler 0 3 1 1 > testresults/3/noStats/0311 &
./profiler 0 3 2 1 > testresults/3/noStats/0321 &
./profiler 0 3 3 1 > testresults/3/noStats/0331 &
./profiler 0 3 4 1 > testresults/3/noStats/0341 &
./profiler 0 3 5 1 > testresults/3/noStats/0351 &
./profiler 0 3 6 1 > testresults/3/noStats/0361 &
#	Policy 1
./profiler 1 3 0 1 > testresults/3/noStats/1301 &
./profiler 1 3 1 1 > testresults/3/noStats/1311 &
./profiler 1 3 2 1 > testresults/3/noStats/1321 &
./profiler 1 3 3 1 > testresults/3/noStats/1331 &
./profiler 1 3 4 1 > testresults/3/noStats/1341 &
./profiler 1 3 5 1 > testresults/3/noStats/1351 &
./profiler 1 3 6 1 > testresults/3/noStats/1361 &
#	Policy 2
./profiler 2 3 0 1 > testresults/3/noStats/2301 &
./profiler 2 3 1 1 > testresults/3/noStats/2311 &
./profiler 2 3 2 1 > testresults/3/noStats/2321 &
./profiler 2 3 3 1 > testresults/3/noStats/2331 &
./profiler 2 3 4 1 > testresults/3/noStats/2341 &
./profiler 2 3 5 1 > testresults/3/noStats/2351 &
./profiler 2 3 6 1 > testresults/3/noStats/2361 &

# Test 4
#	Policy 0
./profiler 0 4 0 1 > testresults/4/noStats/0401 &
./profiler 0 4 1 1 > testresults/4/noStats/0411 &
./profiler 0 4 2 1 > testresults/4/noStats/0421 &
./profiler 0 4 3 1 > testresults/4/noStats/0431 &
./profiler 0 4 4 1 > testresults/4/noStats/0441 &
./profiler 0 4 5 1 > testresults/4/noStats/0451 &
./profiler 0 4 6 1 > testresults/4/noStats/0461 &
#	Policy 1
./profiler 1 4 0 1 > testresults/4/noStats/1401 &
./profiler 1 4 1 1 > testresults/4/noStats/1411 &
./profiler 1 4 2 1 > testresults/4/noStats/1421 &
./profiler 1 4 3 1 > testresults/4/noStats/1431 &
./profiler 1 4 4 1 > testresults/4/noStats/1441 &
./profiler 1 4 5 1 > testresults/4/noStats/1451 &
./profiler 1 4 6 1 > testresults/4/noStats/1461 &
#	Policy 2
./profiler 2 4 0 1 > testresults/4/noStats/2401 &
./profiler 2 4 1 1 > testresults/4/noStats/2411 &
./profiler 2 4 2 1 > testresults/4/noStats/2421 &
./profiler 2 4 3 1 > testresults/4/noStats/2431 &
./profiler 2 4 4 1 > testresults/4/noStats/2441 &
./profiler 2 4 5 1 > testresults/4/noStats/2451 &
./profiler 2 4 6 1 > testresults/4/noStats/2461 &

# Test 5
#	Policy 0
./profiler 0 5 0 1 > testresults/5/noStats/0501 &
./profiler 0 5 1 1 > testresults/5/noStats/0511 &
./profiler 0 5 2 1 > testresults/5/noStats/0521 &
./profiler 0 5 3 1 > testresults/5/noStats/0531 &
./profiler 0 5 4 1 > testresults/5/noStats/0541 &
./profiler 0 5 5 1 > testresults/5/noStats/0551 &
./profiler 0 5 6 1 > testresults/5/noStats/0561 &
#	Policy 1
./profiler 1 5 0 1 > testresults/5/noStats/1501 &
./profiler 1 5 1 1 > testresults/5/noStats/1511 &
./profiler 1 5 2 1 > testresults/5/noStats/1521 &
./profiler 1 5 3 1 > testresults/5/noStats/1531 &
./profiler 1 5 4 1 > testresults/5/noStats/1541 &
./profiler 1 5 5 1 > testresults/5/noStats/1551 &
./profiler 1 5 6 1 > testresults/5/noStats/1561 &
#	Policy 2
./profiler 2 5 0 1 > testresults/5/noStats/2501 &
./profiler 2 5 1 1 > testresults/5/noStats/2511 &
./profiler 2 5 2 1 > testresults/5/noStats/2521 &
./profiler 2 5 3 1 > testresults/5/noStats/2531 &
./profiler 2 5 4 1 > testresults/5/noStats/2541 &
./profiler 2 5 5 1 > testresults/5/noStats/2551 &
./profiler 2 5 6 1 > testresults/5/noStats/2561 &

# Test 6
#	Policy 0
./profiler 0 6 0 1 > testresults/6/noStats/0601 &
./profiler 0 6 1 1 > testresults/6/noStats/0611 &
./profiler 0 6 2 1 > testresults/6/noStats/0621 &
./profiler 0 6 3 1 > testresults/6/noStats/0631 &
./profiler 0 6 4 1 > testresults/6/noStats/0641 &
./profiler 0 6 5 1 > testresults/6/noStats/0651 &
./profiler 0 6 6 1 > testresults/6/noStats/0661 &
#	Policy 1
./profiler 1 6 0 1 > testresults/6/noStats/1601 &
./profiler 1 6 1 1 > testresults/6/noStats/1611 &
./profiler 1 6 2 1 > testresults/6/noStats/1621 &
./profiler 1 6 3 1 > testresults/6/noStats/1631 &
./profiler 1 6 4 1 > testresults/6/noStats/1641 &
./profiler 1 6 5 1 > testresults/6/noStats/1651 &
./profiler 1 6 6 1 > testresults/6/noStats/1661 &
#	Policy 2
./profiler 2 6 0 1 > testresults/6/noStats/2601 &
./profiler 2 6 1 1 > testresults/6/noStats/2611 &
./profiler 2 6 2 1 > testresults/6/noStats/2621 &
./profiler 2 6 3 1 > testresults/6/noStats/2631 &
./profiler 2 6 4 1 > testresults/6/noStats/2641 &
./profiler 2 6 5 1 > testresults/6/noStats/2651 &
./profiler 2 6 6 1 > testresults/6/noStats/2661 &

# Test 7
#	Policy 0
./profiler 0 7 0 1 > testresults/7/noStats/0501 &
./profiler 0 7 1 1 > testresults/7/noStats/0511 &
./profiler 0 7 2 1 > testresults/7/noStats/0521 &
./profiler 0 7 3 1 > testresults/7/noStats/0531 &
./profiler 0 7 4 1 > testresults/7/noStats/0541 &
./profiler 0 7 5 1 > testresults/7/noStats/0551 &
./profiler 0 7 6 1 > testresults/7/noStats/0561 &
#	Policy 1
./profiler 1 7 0 1 > testresults/7/noStats/1701 &
./profiler 1 7 1 1 > testresults/7/noStats/1711 &
./profiler 1 7 2 1 > testresults/7/noStats/1721 &
./profiler 1 7 3 1 > testresults/7/noStats/1731 &
./profiler 1 7 4 1 > testresults/7/noStats/1741 &
./profiler 1 7 5 1 > testresults/7/noStats/1751 &
./profiler 1 7 6 1 > testresults/7/noStats/1761 &
#	Policy 2
./profiler 2 7 0 1 > testresults/7/noStats/2701 &
./profiler 2 7 1 1 > testresults/7/noStats/2711 &
./profiler 2 7 2 1 > testresults/7/noStats/2721 &
./profiler 2 7 3 1 > testresults/7/noStats/2731 &
./profiler 2 7 4 1 > testresults/7/noStats/2741 &
./profiler 2 7 5 1 > testresults/7/noStats/2751 &
./profiler 2 7 6 1 > testresults/7/noStats/2761 &
