#!/bin/bash

gcc prod_mat.c -o prod_mat.out -fopenmp

if [ -f "prod_mat.out" ];
then
	./prod_mat.out
else
	echo "Error while compiling"
fi
