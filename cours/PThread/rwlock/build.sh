#!/bin/sh

mkdir bin

for i in *.c; do
	echo "`basename $i .c` $i"	
	gcc -g -o bin/`basename $i .c` $i -lpthread
done

for i in correction/*.c; do
	echo "correction_`basename $i .c` $i"	
	gcc -g -o bin/correction_`basename $i .c` $i -lpthread
done
