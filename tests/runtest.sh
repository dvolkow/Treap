#!/bin/bash -eu

OPERATION_COUNT=50
RANGE=10
count=0
LIMIT=$1
EXEPTION="EXEPTION"

#---cборка необходимого:
make newtest
make simplejava
make treap

gen()
{
    echo "***** Generation: $count"
 	./gentest $count $OPERATION_COUNT $RANGE 
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

	if [[ `cat output_java.txt | grep "EXEPTION"` =~ $EXEPTION ]]; then
		echo $EXEPTION
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
