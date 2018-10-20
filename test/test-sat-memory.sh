#!/bin/bash

# CHECK DIRECTORY OF EXAMPLES
if [ ! -d "$1" ] ; then
	echo -e "\e[39mdirectory \"$1\" not found!"
	exit
fi

# COMPILE SOURCE FILES AND GENERATE AN EXECUTABLE
echo -e "\e[39mCompiling source files..."
c=`gcc -I/usr/include -L/usr/lib ../src/main.c ../src/io.h ../src/io.c ../src/structures.c ../src/structures.h ../src/sat.h ../src/sat.c -o sat`

# If there is a fail in compilation, exit
if [ "$c" != "" ] ; then
	echo -e "\e[39mfail!"
	exit
else
	echo -e "\e[39mdone!"
fi

# TEST SAT PROGRAMS FROM $1 DIRECTORY WITH VALGRIND
echo -e "\e[39mTesting SAT programs with valgrind..."

# Number of tests
i=0
# Total number of errors
errors=0
# Total number of success
success=0
# Total number of valgrin errors
leaks=0

# Iterate files from /examples directory
for p in `find $1 -name \*.cnf -print`; do

	# Update number of files
	i=$((i+1))
	
	# Run SAT solver in valgrind with timeout
	c=`timeout ${2:-2}s valgrind --log-file=sat.log ./sat "$p"`
	
	# Get summary
	summary=`tail -n 1 sat.log | cut -d$':' -f2 | cut -d$'(' -f1`
	
	# Get number of valgrin errors
	leak=`echo $summary | cut -d$' ' -f1`
	leaks=$((leak+leaks))
	
	# If SAT program ends
	if [ "$c" != "" ] ; then
		# Get satisfiability status (SAT or UNSAT)
		sat=`echo $c | cut -d$' ' -f1`
		# Update status if there is an error in the execution
		if [ "$sat" != "SAT" ] && [ "$sat" != "UNSAT" ] ; then
			sat="ERROR"
		fi
	# If SAT program doesn't end
	else
		sat="TIMEOUT"
	fi
	
	# Print status
	if [ $leak -eq 0 ] ; then
		echo -e "\e[39m($i) \e[32m$p $sat!\e[39m"
		success=$((success+1))
	else
		echo -e "\e[39m($i) \e[31m$p $sat! $leak ERRORS! \e[39m"
		errors=$((errors+1))
	fi

done

echo -e "\e[39mdone!\e[39m"

# Print statistics
echo -e "\e[32m$success SUCCESS!\e[39m"
echo -e "\e[31m$leaks ERRORS IN $errors PROGRAMS!\e[39m"
