#!/bin/sh
cd ./memAllocLib
echo "\n========================================================================="
echo "Attempting to build libMM..."
echo "=========================================================================\n"
make
cd ../profiler
echo "\n========================================================================="
echo "Attempting to copy (or copy and override) libMM.a to \"./profiler/src/include\""
echo "Attempting to copy (or copy and override) mm.h to \"./profiler/src/include\""
echo "=========================================================================\n"
cp -f ../memAllocLib/libMM.a ./src/include
cp -f ../memAllocLib/src/include/mm.h ./src/include
echo "\n========================================================================="
echo "Attempting to build profiler..."
echo "=========================================================================\n"
make
