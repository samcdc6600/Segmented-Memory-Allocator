#!/bin/sh
# Veriables
allocatorLib="libMM.a"
allocatorHeaderCpp="mm.hpp"
allocatorHeaderC="mm.h"
lineIdentifier="\\(\\[-{REMOVE--ME}-\\]\\)"
# The un-escaped string is as follows:
# ||||~|~|([-{REMOVE--ME}-])|~|~||||
# Actions
cd ./memAllocLib
echo "\n========================================================================="
echo "Attempting to build libMM..."
echo "=========================================================================\n"
make
cd ../profiler
echo "\n========================================================================="
echo "Attempting to copy (or copy and override) $allocatorLib to\n\"./profiler/src/include\"."
echo "Attempting to copy (or copy and override) $allocatorHeaderCpp to \n\"./profiler/src/include/$allocatorHeaderC\" whilst also attempting to remove lines\nmarked with \"$lineIdentifier\" (where '\' characters are removed by\nsed) from $allocatorHeaderC to make it compatible with the C language syntax."
echo "=========================================================================\n"
cp -f ../memAllocLib/$allocatorLib ./src/include
sed '/'$lineIdentifier'/d' ../memAllocLib/src/include/$allocatorHeaderCpp > ./src/include/$allocatorHeaderC
echo "\n========================================================================="
echo "Attempting to build profiler..."
echo "=========================================================================\n"
make
