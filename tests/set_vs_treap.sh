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
if [[ "$OPERATION_COUNT" -lt "$TRESHOLD"  ]]; then
	make 'utest' #---последовательная 
else
	make 'paralleltest' #---параллельная 
fi

make 'debug_set_vs_treap'
make 'set'

gen()
{
    echo "***** Generation: $count"
 	time ./gentest $count $OPERATION_COUNT $RANGE 
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
