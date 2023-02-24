#!/bin/bash

gcc reduction_tab_old.c -o reduction_tab_old.out -fopenmp && ./reduction_tab_old.out
gcc reduction_tab.c -o reduction_tab.out -fopenmp && ./reduction_tab.out

if [ -f "pi_mc.out" ];
then
	./pi_mc.out
else
	echo "Error while compiling"
fi
