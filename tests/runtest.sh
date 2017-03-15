#!/bin/bash -eu

if [[ "$#" -lt 2 ]]; then
	OPERATION_COUNT=42
	else
	OPERATION_COUNT=$2
fi

if [[ "$#" -lt 3 ]]; then
	RANGE=42
	else
	RANGE=$3
fi

count=0
LIMIT=$1
EXCEPTION="EXCEPTION"
ex_counter=0
TRESHOLD=1000000

#---cборка необходимого:
#---cборка необходимого:
if [[ "$OPERATION_COUNT" -lt "$TRESHOLD"  ]]; then
	make 'utest' #---последовательная 
else
	make 'paralleltest' #---параллельная 
fi

make 'debug_casual'
make 'simplejava'

gen()
{
    echo "***** Generation: $count"
 	time ./gentest $count $OPERATION_COUNT $RANGE 
	echo "============================================="
    return $?
}

run()
{
    echo "***** Run: $count"
	time ./treap
    return $?

}

java_run()
{
	echo "***** Java: $count"
	time java SimpleTree
    return $?
}

while [ "$count" -lt "$LIMIT" ] 
do
	gen
	run
	java_run

	if [[ `cat output_java.txt | grep "EXCEPTION"` =~ $EXCEPTION ]]; then
		echo $EXCEPTION
		ex_counter=`expr $ex_counter + 1` 
	else	
	    diff -q output.txt output_java.txt
	    echo "Success!"
	fi

	count=`expr $count + 1`
	echo "============================================="
done

#----уборка
make clean 

echo "Test passed!"
echo "Exceptions: $ex_counter / $LIMIT"
