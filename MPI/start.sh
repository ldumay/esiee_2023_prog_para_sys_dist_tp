#!/bin/bash

# Get filename to compile (default is hello.c)
filename=${1:-"hello"}

if [ -f $filename.c ];
then
	if [ -f $filename.out ];
	then
		# Delete out file to regenerate and avoid execution if an error occured
		rm ./$filename.out
	fi
	mpicc $filename.c -o $filename.out
else
	echo "File $filename.c not found"
fi

if [ -f $filename.out ];
then
	# Use a second argument to change the number of process run
	mpirun --hostfile hosts -np ${2:-2} $filename.out
else
	echo "Error while compiling $filename.c"
fi

