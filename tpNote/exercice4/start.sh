#!/bin/bash

gcc -Wall pi_mc.c random.c -o pi_mc.out -fopenmp

if [ -f "pi_mc.out" ];
then
	./pi_mc.out
else
	echo "Error while compiling"
fi
