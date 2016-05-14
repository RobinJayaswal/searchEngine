#!/bin/bash
#
# querierArgTest.sh
# Tests the argument validation of querier, providing it with a variety
# of invalid commands
#

if [ $# != 2 ]; then
	echo usage: querierArgTest pageDirectory indexFilename
	exit 1
else
	pageDir="$1"
	index="$2"
fi

# Test for invalid number of args to querier
echo running \'querier \' with no args.
echo
echo output:
./querier
if [ $? != 0 ]; then
	echo
	echo PASSED: exited with error
else
	echo
	echo FAILURE: did not exit with error
	exit 10
fi
echo

echo running \'querier $pageDir\', one too few args.
echo
echo output:
./querier "$pageDir"

if [ $? != 0 ]; then
	echo
	echo PASSED: exited with error
else
	echo
	echo FAILURE: did not exit with error
	exit 11
fi
echo

echo running \'querier $pageDir $indexFilename extraArg\'
echo
echo output:
./querier "$pageDir" "$indexFilename" extraArg
if [ $? != 0 ]; then
	echo
	echo PASSED: exited with error
else
	echo
	echo FAILURE: did not exit with error
	exit 12
fi
echo 

echo running \'querier this/is/not/a/pageDir $indexFilename\', bad pageDirectory
echo
echo output:
./querier this/is/not/a/pageDir "$indexFilename"
if [ $? != 0 ]; then
	echo
	echo PASSED: exited with error
else
	echo
	echo FAILURE: did not exit with error
	exit 14
fi
echo 

echo running \'querier $pageDir filename8That8Does8Exist\', bad indexFilename
echo
echo output:
./querier "$pageDir" filename8That8Does8Exist

if [ $? != 0 ]; then
	echo
	echo PASSED: exited with error
else
	echo
	echo FAILURE: did not exit with error
	exit 15
fi
echo
echo All Tests PASSED





