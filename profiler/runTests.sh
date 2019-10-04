#!/bin/sh

for policy in 0 1 2			# Memory allocation policy, where the range is [0, 2]
do
    for testNum in 0 1 2 3 4 5 6 7 # Specific test, where the range is [0, 7]
    do
	for sizeNum in 0 1 2 3 4 #5 6 # Number of allocations to perform, where the range is [0, 6]
	do
	    for listStat in 0 1	# Output extra statistics about the inUse and holes lists when 1. There is a choice
	    do			# here because there is a run time overhead associated with gathering the extra
		# statistics that affects the other statistics.
		if test $listStat -eq 0
		then
		    STATS="normalStats"
		else
		    STATS="extraStats"
		fi
		mkdir -p testresults/$testNum/$STATS # Make directory
		# Run tests
		./profiler $policy $testNum $sizeNum $listStat > testresults/$testNum/$STATS/$policy$testNum$sizeNum$listStat
	    done
	    done
    done
done
