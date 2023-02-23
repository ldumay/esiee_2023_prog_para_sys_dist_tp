#!/bin/bash

gcc pi_mc.c -o pi_mc.out -fopenmp

if [ -f "pi_mc.out" ];
then
	./pi_mc.out
else
	echo "Error while compiling"
fi
