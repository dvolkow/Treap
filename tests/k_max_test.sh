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
TRESHOLD=1000000

#---cборка необходимого:

make 'k_max_test'

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

while [ "$count" -lt "$LIMIT" ] 
do
	gen
	run

	count=`expr $count + 1`
	echo "============================================="
done

echo "Test passed!"
