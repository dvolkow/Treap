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

make 'debug_set_vs_treap'
make 'set'
make 'simplejava'

gen()
{
    echo "***** Generation: $count"
 	time ./gentest $count $OPERATION_COUNT $RANGE 
	echo "============================================="
    return $?
}

run_treap()
{
    echo "***** Run : TREAP # $count"
	time ./treap
    return $?

}

run_set()
{
    echo "***** Run: SET # $count"
	time './set'
    return $?

}

run_java()
{
	echo "***** Run: JAVA_STUPID_TREE # $count"
	time java SimpleTree
    return $?
}

while [ "$count" -lt "$LIMIT" ] 
do
	gen 
	run_treap
	run_set 
 	run_java

	if [[ `cat output_java.txt | grep "EXCEPTION"` =~ $EXCEPTION ]]; then
		echo "$EXCEPTION, test failed!"
		ex_counter=`expr $ex_counter + 1` 
 	else	
		echo "Compare result: treap & stupid_BST:"
	    diff -q output_treap.txt output_java.txt
		echo "Success!"
		echo "Compare result: set & treap:"
		diff -q output_set.txt output_treap.txt
		echo "Success!"
	fi


	count=`expr $count + 1`
	echo "============================================="
done

#----уборка
make clean 

echo "Test passed!"
echo "Exceptions: $ex_counter / $LIMIT"
