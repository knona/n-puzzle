#!/bin/bash
wait=true
if [ "$1" == "--no-waiting" ] || [ "$2" == "--no-waiting" ]
then
	wait=false
fi

valgrind=false
if [ "$1" == "--valgrind" ] || [ "$2" == "--valgrind" ]
then
	valgrind=true
fi

make
for f in tests/parsing/files/* 
do
	echo -ne "\033[32m"
	head -n 1 $f
	echo -ne "\033[0m"
	tail -n +2 $f
	echo -e "\033[33m---------\033[0m"
	if [ "$valgrind" == true ]
	then
		valgrind --leak-check=full -q ./n-puzzle --file=$f --parse-only
	else
		./n-puzzle --file=$f --parse-only
	fi
	if [ "$wait" == true ]
	then
		read
	fi
	echo
done
