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

# TEST SAT PROGRAMS FROM /examples DIRECTORY
echo -e "\e[39mTesting SAT programs..."

# Number of tests
i=0
# Total number of errors
errors=0
# Total number of success
success=0
# Total number of timeouts
timeout=0

# Iterate files from /examples directory
for p in `find $1 -name \*.cnf -print`; do

	# Update number of files
	i=$((i+1))
	
	# Run SAT solver with timeout
	c=`timeout ${2:-1}s ./sat "$p"`
	
	# If SAT program ends
	if [ "$c" != "" ] ; then
		# Get satisfiability status (SAT or UNSAT)
		s=`echo $c | cut -d$' ' -f1`
		# Get model (if exists)
		m=`echo .$c | cut -d$' ' -f2-`
		# Print status (ERROR, SAT or UNSAT)
		if [ "$s" != "SAT" ] && [ "$s" != "UNSAT" ] ; then
			echo -e "\e[39m($i) \e[31m[$p ERROR!\e[39m"
			errors=$((errors+1))
		else
			echo -e "\e[39m($i) \e[32m$p $s!\e[39m"
			success=$((success+1))
		fi
	# If SAT program doesn't end
	else
		echo -e "\e[39m($i) \e[96m$p TIMEOUT!\e[39m"
		timeout=$((timeout+1))
	fi
	
done

echo -e "\e[39mdone!\e[39m"

# Print statistics
echo -e "\e[32m$success SUCCESS!\e[39m"
echo -e "\e[31m$errors ERRORS!\e[39m"
echo -e "\e[96m$timeout TIMEOUTS!\e[39m"
