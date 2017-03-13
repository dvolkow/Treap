#!/bin/bash -eu

count=0
limit=$1
EXEPTION="EXEPTION"

gen()
{
    echo "***** Generation: $count"
 	./gentest $count
# 	sleep 0.7
	echo "============================================="
    return $?
}

run()
{
    echo "***** Run: $count"
	time ./treap
#	sleep 0.7
    return $?

}

java_run()
{
	echo "***** Java: $count"
	time java SimpleTree
#	sleep 1
    return $?
}

while [ "$count" -lt "$limit" ] 
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

echo "Test passed!"
