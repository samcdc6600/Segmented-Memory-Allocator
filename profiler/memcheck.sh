#!/bin/sh
valgrind --leak-check=full -v ./profiler $1 $2 $3
