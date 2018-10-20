#!/bin/bash

# Compile source files and generate an executable
echo "Compiling source files..."
c=`gcc -I/usr/include -L/usr/lib ../src/main.c ../src/io.h ../src/io.c ../src/structures.c ../src/structures.h ../src/sat.h ../src/sat.c -o sat`

if [ "$c" != "" ] ; then
	echo "FAIL!"
	exit
else
	echo "DONE!"
fi

# Test SAT programs from /examples directory
echo "Testing SAT programs..."
i=0
errors=0
success=0
for p in `ls ../examples/`; do
	i=$((i+1))
	c=`./sat "../examples/$p"`
	s=`echo $c | cut -d$' ' -f1`
	m=`echo .$c | cut -d$' ' -f2-`
    echo "....($i) $p"
	if [ "$s" != "SAT" ] && [ "$s" != "UNSAT" ] ; then
		echo "........FAIL!"
		errors=$((errors+1))
	else
		echo "........$s!"
		if [ "$s" = "SAT" ] ; then
			echo "........($m)"
		fi
		echo "........OK!"
		success=$((success+1))
	fi
done
echo "$success SUCCESS!"
echo "$errors ERRORS!"
echo "DONE!"
