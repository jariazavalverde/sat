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
# Total number of timeouts
timeout=0

# Iterate files from /examples directory
for p in `find $1 -name \*.cnf -print`; do

	# Update number of files
	i=$((i+1))
	
	# Run SAT solver in valgrind with timeout
	c=`timeout ${2:-2}s valgrind --log-file=sat.log ./sat "$p"`
	
	# Get summary
	summary=`tail -n 1 sat.log | cut -d$':' -f2 | cut -d$'(' -f1`
	
	# If SAT program ends
	if [ "$c" != "" ] ; then
		# Get satisfiability status (SAT or UNSAT)
		sat=`echo $c | cut -d$' ' -f1`
		# Print status (ERROR, SAT or UNSAT) and summary
		if [ "$sat" != "SAT" ] && [ "$sat" != "UNSAT" ] ; then
			echo -e "\e[39m($i) \e[31m$p ERROR!$summary\e[39m"
			errors=$((errors+1))
		else
			echo -e "\e[39m($i) \e[32m$p $sat!$summary\e[39m"
			success=$((success+1))
		fi
	# If SAT program doesn't end
	else
		echo -e "\e[39m($i) \e[96m$p TIMEOUT!$summary\e[39m"
		timeout=$((timeout+1))
	fi
	
done

echo -e "\e[39mdone!\e[39m"

# Print statistics
echo -e "\e[32m$success SUCCESS!\e[39m"
echo -e "\e[31m$errors ERRORS!\e[39m"
echo -e "\e[96m$timeout TIMEOUTS!\e[39m"
