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

make 'debug_RE'
#make 'dbg_t'

run()
{
    echo "***** Run: $count"
	time ./treap $OPERATION_COUNT $RANGE
    return $?
}

while [ "$count" -lt "$LIMIT" ] 
do
	run

	count=`expr $count + 1`
	echo "============================================="
done

echo "Test passed!"
