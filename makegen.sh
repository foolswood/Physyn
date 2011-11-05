#!/bin/bash

gen_entry() {
	includes=`cat $1 | grep "#include \"" | cut -d \" -f 2`
	IFS="
"
	basename=`echo "$1" | cut -d '.' -f 1`
	echo -n "$basename".o "$basename".h ": $1 "
	for item in $includes; do
		echo -n "$item "
	done
	echo
	echo "	./headergen.py $1"
	echo "	gcc -Wall -ggdb -c $1"
	echo
}

gen_entries() {
	while [ -n "$1" ]; do
		if [ "$1" != "physyn.c" ]; then
			gen_entry "$1"
		fi
		shift
	done
}
cat makefile_head
gen_entries *.c
cat makefile_tail
