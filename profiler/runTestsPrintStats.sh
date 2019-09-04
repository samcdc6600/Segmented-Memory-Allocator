#!/bin/sh
# Tests with additonal printStats() output
# Test 0
#	Policy 0
./profiler 0 0 0 1 > testresults/0/printStats/0001 &
./profiler 0 0 1 1 > testresults/0/printStats/0011 &
./profiler 0 0 2 1 > testresults/0/printStats/0021 &
./profiler 0 0 3 1 > testresults/0/printStats/0031 &
./profiler 0 0 4 1 > testresults/0/printStats/0041 &
./profiler 0 0 5 1 > testresults/0/printStats/0051 &
./profiler 0 0 6 1 > testresults/0/printStats/0061 &
#	Policy 1
./profiler 1 0 0 1 > testresults/0/printStats/1001 &
./profiler 1 0 1 1 > testresults/0/printStats/1011 &
./profiler 1 0 2 1 > testresults/0/printStats/1021 &
./profiler 1 0 3 1 > testresults/0/printStats/1031 &
./profiler 1 0 4 1 > testresults/0/printStats/1041 &
./profiler 1 0 5 1 > testresults/0/printStats/1051 &
./profiler 1 0 6 1 > testresults/0/printStats/1061 &
#	Policy 2
./profiler 2 0 0 1 > testresults/0/printStats/2001 &
./profiler 2 0 1 1 > testresults/0/printStats/2011 &
./profiler 2 0 2 1 > testresults/0/printStats/2021 &
./profiler 2 0 3 1 > testresults/0/printStats/2031 &
./profiler 2 0 4 1 > testresults/0/printStats/2041 &
./profiler 2 0 5 1 > testresults/0/printStats/2051 &
./profiler 2 0 6 1 > testresults/0/printStats/2061 &

# Test 1
#	Policy 0
./profiler 0 1 0 1 > testresults/1/printStats/0101 &
./profiler 0 1 1 1 > testresults/1/printStats/0111 &
./profiler 0 1 2 1 > testresults/1/printStats/0121 &
./profiler 0 1 3 1 > testresults/1/printStats/0131 &
./profiler 0 1 4 1 > testresults/1/printStats/0141 &
./profiler 0 1 5 1 > testresults/1/printStats/0151 &
./profiler 0 1 6 1 > testresults/1/printStats/0161 &
#	Policy 1
./profiler 1 1 0 1 > testresults/1/printStats/1101 &
./profiler 1 1 1 1 > testresults/1/printStats/1111 &
./profiler 1 1 2 1 > testresults/1/printStats/1121 &
./profiler 1 1 3 1 > testresults/1/printStats/1131 &
./profiler 1 1 4 1 > testresults/1/printStats/1141 &
./profiler 1 1 5 1 > testresults/1/printStats/1151 &
./profiler 1 1 6 1 > testresults/1/printStats/1161 &
#	Policy 2
./profiler 2 1 0 1 > testresults/1/printStats/2101 &
./profiler 2 1 1 1 > testresults/1/printStats/2111 &
./profiler 2 1 2 1 > testresults/1/printStats/2121 &
./profiler 2 1 3 1 > testresults/1/printStats/2131 &
./profiler 2 1 4 1 > testresults/1/printStats/2141 &
./profiler 2 1 5 1 > testresults/1/printStats/2151 &
./profiler 2 1 6 1 > testresults/1/printStats/2161 &

# Test 2
#	Policy 0
./profiler 0 2 0 1 > testresults/2/printStats/0201 &
./profiler 0 2 1 1 > testresults/2/printStats/0211 &
./profiler 0 2 2 1 > testresults/2/printStats/0221 &
./profiler 0 2 3 1 > testresults/2/printStats/0231 &
./profiler 0 2 4 1 > testresults/2/printStats/0241 &
./profiler 0 2 5 1 > testresults/2/printStats/0251 &
./profiler 0 2 6 1 > testresults/2/printStats/0261 &
#	Policy 1
./profiler 1 2 0 1 > testresults/2/printStats/1201 &
./profiler 1 2 1 1 > testresults/2/printStats/1211 &
./profiler 1 2 2 1 > testresults/2/printStats/1221 &
./profiler 1 2 3 1 > testresults/2/printStats/1231 &
./profiler 1 2 4 1 > testresults/2/printStats/1241 &
./profiler 1 2 5 1 > testresults/2/printStats/1251 &
./profiler 1 2 6 1 > testresults/2/printStats/1261 &
#	Policy 2
./profiler 2 2 0 1 > testresults/2/printStats/2201 &
./profiler 2 2 1 1 > testresults/2/printStats/2211 &
./profiler 2 2 2 1 > testresults/2/printStats/2221 &
./profiler 2 2 3 1 > testresults/2/printStats/2231 &
./profiler 2 2 4 1 > testresults/2/printStats/2241 &
./profiler 2 2 5 1 > testresults/2/printStats/2251 &
./profiler 2 2 6 1 > testresults/2/printStats/2261 &

# Test 3
#	Policy 0
./profiler 0 3 0 1 > testresults/3/printStats/0301 &
./profiler 0 3 1 1 > testresults/3/printStats/0311 &
./profiler 0 3 2 1 > testresults/3/printStats/0321 &
./profiler 0 3 3 1 > testresults/3/printStats/0331 &
./profiler 0 3 4 1 > testresults/3/printStats/0341 &
./profiler 0 3 5 1 > testresults/3/printStats/0351 &
./profiler 0 3 6 1 > testresults/3/printStats/0361 &
#	Policy 1
./profiler 1 3 0 1 > testresults/3/printStats/1301 &
./profiler 1 3 1 1 > testresults/3/printStats/1311 &
./profiler 1 3 2 1 > testresults/3/printStats/1321 &
./profiler 1 3 3 1 > testresults/3/printStats/1331 &
./profiler 1 3 4 1 > testresults/3/printStats/1341 &
./profiler 1 3 5 1 > testresults/3/printStats/1351 &
./profiler 1 3 6 1 > testresults/3/printStats/1361 &
#	Policy 2
./profiler 2 3 0 1 > testresults/3/printStats/2301 &
./profiler 2 3 1 1 > testresults/3/printStats/2311 &
./profiler 2 3 2 1 > testresults/3/printStats/2321 &
./profiler 2 3 3 1 > testresults/3/printStats/2331 &
./profiler 2 3 4 1 > testresults/3/printStats/2341 &
./profiler 2 3 5 1 > testresults/3/printStats/2351 &
./profiler 2 3 6 1 > testresults/3/printStats/2361 &

# Test 4
#	Policy 0
./profiler 0 4 0 1 > testresults/4/printStats/0401 &
./profiler 0 4 1 1 > testresults/4/printStats/0411 &
./profiler 0 4 2 1 > testresults/4/printStats/0421 &
./profiler 0 4 3 1 > testresults/4/printStats/0431 &
./profiler 0 4 4 1 > testresults/4/printStats/0441 &
./profiler 0 4 5 1 > testresults/4/printStats/0451 &
./profiler 0 4 6 1 > testresults/4/printStats/0461 &
#	Policy 1
./profiler 1 4 0 1 > testresults/4/printStats/1401 &
./profiler 1 4 1 1 > testresults/4/printStats/1411 &
./profiler 1 4 2 1 > testresults/4/printStats/1421 &
./profiler 1 4 3 1 > testresults/4/printStats/1431 &
./profiler 1 4 4 1 > testresults/4/printStats/1441 &
./profiler 1 4 5 1 > testresults/4/printStats/1451 &
./profiler 1 4 6 1 > testresults/4/printStats/1461 &
#	Policy 2
./profiler 2 4 0 1 > testresults/4/printStats/2401 &
./profiler 2 4 1 1 > testresults/4/printStats/2411 &
./profiler 2 4 2 1 > testresults/4/printStats/2421 &
./profiler 2 4 3 1 > testresults/4/printStats/2431 &
./profiler 2 4 4 1 > testresults/4/printStats/2441 &
./profiler 2 4 5 1 > testresults/4/printStats/2451 &
./profiler 2 4 6 1 > testresults/4/printStats/2461 &

# Test 5
#	Policy 0
./profiler 0 5 0 1 > testresults/5/printStats/0501 &
./profiler 0 5 1 1 > testresults/5/printStats/0511 &
./profiler 0 5 2 1 > testresults/5/printStats/0521 &
./profiler 0 5 3 1 > testresults/5/printStats/0531 &
./profiler 0 5 4 1 > testresults/5/printStats/0541 &
./profiler 0 5 5 1 > testresults/5/printStats/0551 &
./profiler 0 5 6 1 > testresults/5/printStats/0561 &
#	Policy 1
./profiler 1 5 0 1 > testresults/5/printStats/1501 &
./profiler 1 5 1 1 > testresults/5/printStats/1511 &
./profiler 1 5 2 1 > testresults/5/printStats/1521 &
./profiler 1 5 3 1 > testresults/5/printStats/1531 &
./profiler 1 5 4 1 > testresults/5/printStats/1541 &
./profiler 1 5 5 1 > testresults/5/printStats/1551 &
./profiler 1 5 6 1 > testresults/5/printStats/1561 &
#	Policy 2
./profiler 2 5 0 1 > testresults/5/printStats/2501 &
./profiler 2 5 1 1 > testresults/5/printStats/2511 &
./profiler 2 5 2 1 > testresults/5/printStats/2521 &
./profiler 2 5 3 1 > testresults/5/printStats/2531 &
./profiler 2 5 4 1 > testresults/5/printStats/2541 &
./profiler 2 5 5 1 > testresults/5/printStats/2551 &
./profiler 2 5 6 1 > testresults/5/printStats/2561 &

# Test 6
#	Policy 0
./profiler 0 6 0 1 > testresults/6/printStats/0601 &
./profiler 0 6 1 1 > testresults/6/printStats/0611 &
./profiler 0 6 2 1 > testresults/6/printStats/0621 &
./profiler 0 6 3 1 > testresults/6/printStats/0631 &
./profiler 0 6 4 1 > testresults/6/printStats/0641 &
./profiler 0 6 5 1 > testresults/6/printStats/0651 &
./profiler 0 6 6 1 > testresults/6/printStats/0661 &
#	Policy 1
./profiler 1 6 0 1 > testresults/6/printStats/1601 &
./profiler 1 6 1 1 > testresults/6/printStats/1611 &
./profiler 1 6 2 1 > testresults/6/printStats/1621 &
./profiler 1 6 3 1 > testresults/6/printStats/1631 &
./profiler 1 6 4 1 > testresults/6/printStats/1641 &
./profiler 1 6 5 1 > testresults/6/printStats/1651 &
./profiler 1 6 6 1 > testresults/6/printStats/1661 &
#	Policy 2
./profiler 2 6 0 1 > testresults/6/printStats/2601 &
./profiler 2 6 1 1 > testresults/6/printStats/2611 &
./profiler 2 6 2 1 > testresults/6/printStats/2621 &
./profiler 2 6 3 1 > testresults/6/printStats/2631 &
./profiler 2 6 4 1 > testresults/6/printStats/2641 &
./profiler 2 6 5 1 > testresults/6/printStats/2651 &
./profiler 2 6 6 1 > testresults/6/printStats/2661 &

# Test 7
#	Policy 0
./profiler 0 7 0 1 > testresults/7/printStats/0501 &
./profiler 0 7 1 1 > testresults/7/printStats/0511 &
./profiler 0 7 2 1 > testresults/7/printStats/0521 &
./profiler 0 7 3 1 > testresults/7/printStats/0531 &
./profiler 0 7 4 1 > testresults/7/printStats/0541 &
./profiler 0 7 5 1 > testresults/7/printStats/0551 &
./profiler 0 7 6 1 > testresults/7/printStats/0561 &
#	Policy 1
./profiler 1 7 0 1 > testresults/7/printStats/1701 &
./profiler 1 7 1 1 > testresults/7/printStats/1711 &
./profiler 1 7 2 1 > testresults/7/printStats/1721 &
./profiler 1 7 3 1 > testresults/7/printStats/1731 &
./profiler 1 7 4 1 > testresults/7/printStats/1741 &
./profiler 1 7 5 1 > testresults/7/printStats/1751 &
./profiler 1 7 6 1 > testresults/7/printStats/1761 &
#	Policy 2
./profiler 2 7 0 1 > testresults/7/printStats/2701 &
./profiler 2 7 1 1 > testresults/7/printStats/2711 &
./profiler 2 7 2 1 > testresults/7/printStats/2721 &
./profiler 2 7 3 1 > testresults/7/printStats/2731 &
./profiler 2 7 4 1 > testresults/7/printStats/2741 &
./profiler 2 7 5 1 > testresults/7/printStats/2751 &
./profiler 2 7 6 1 > testresults/7/printStats/2761 &
