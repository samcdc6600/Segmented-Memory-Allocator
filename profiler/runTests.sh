#!/bin/sh

# Memory allocation policy, where the range is [0, 2]
for policy in 0 1 2
do
    # Specific test, where the range is [0, 7]
    for testNum in 0 1 2 3 4 5 6 7
    do
	#5 6 # Number of allocations to perform, where the range is [0, 6]
	for sizeNum in 0 1 2
	do
	    # How many threads to run a test in. I.e., each thread will run a
	    # duplicate test. (The results will still be output to the same
	    # file.)
	    for explicitConcurrencyNumber in 1 2 3 4
	    do
		# Output extra statistics about the inUse and holes lists when
		# 1. There is a choice here because there is a run time overhead
		# associated with gathering the extra statistics that affects
		# the other statistics.
		for listStat in 0 1	
		do			
		    if test $listStat -eq 0
		    then
			STATS="normalStats"
		    else
			STATS="extraStats"
		    fi
		    mkdir -p testresults/$testNum/$STATS # Make directory
		    # Run tests
		    ./profiler $policy $testNum $sizeNum $explicitConcurrencyNumber $listStat > testresults/$testNum/$STATS/$policy$testNum$sizeNum$explicitConcurrencyNumber$listStat
		done
	    done
	done
    done
done
