#!/bin/bash

gcc prod_cons.c -o prod_cons.out -fopenmp

if [ -f "prod_cons.out" ];
then
	export OMP_NUM_THREADS=${1:-4}
	./prod_cons.out
else
	echo "Error while compiling"
fi
