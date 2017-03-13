#!/bin/bash -eu

OPERATION_COUNT=1000000
RANGE=10
count=0
LIMIT=$1

#---cборка необходимого:
make 'newtest'
make 'treap'
make 'set'

gen()
{
    echo "***** Generation: $count"
 	./gentest $count $OPERATION_COUNT $RANGE 
	echo "============================================="
    return $?
}

run_treap()
{
    echo "***** Run treap: $count"
	time ./treap
    return $?

}

run_set()
{
	echo "***** Run set: $count"
	time './set'
    return $?
}

while [ "$count" -lt "$LIMIT" ] 
do
	gen
	run_treap
	run_set

	diff -q output_set.txt output_treap.txt
	echo "Success!"

	count=`expr $count + 1`
	echo "============================================="
done

#----уборка
#make clean 

echo "Test passed!"
