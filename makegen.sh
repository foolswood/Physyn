#!/bin/bash

do_includes() {
	includes=`cat $1 | grep "#include \"" | cut -d \" -f 2`
	IFS="
"
	for item in $includes; do
		echo -n "$item "
	done
}

gen_entry() {
	basename=`echo "$1" | cut -d '.' -f 1`
	#header file
	echo -n "$basename".h ": $1 "
	do_includes $1
	echo
	echo "	./headergen.py $1"
	echo
	#object file
	echo -n "$basename".o ": $1 "
	do_includes $1
	echo
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

do_plugin_includes() {
	includes=`cat $1 | grep "#include \"" | cut -d \" -f 2 | cut -d \/ -f 2`
	IFS="
"
	for item in $includes; do
		echo -n "$item "
	done
}

gen_plugin() {
	basename=`echo "$1" | cut -d '.' -f 1`
	echo -n "$basename".so ": $1 "
	do_plugin_includes $1
	echo
	echo "	gcc -Wall -ggdb -fpic -c $1 -o "$basename".o"
	echo "	gcc -shared -Wl,-soname,"$basename".so -o "$basename".so "$basename".o"
	echo
}

gen_plugins() {
	while [ -n "$1" ]; do
		gen_plugin "$1"
		shift
	done
}

gen_main() {
	echo -n "physyn : physyn.c "
	while [ -n "$1" ]; do
		if [ "$1" != "physyn.c" ]; then
			basename=`echo "$1" | cut -d '.' -f 1`
			echo -n "$basename".o" "
		fi
		shift
	done
	do_includes "physyn.c"
	echo
	echo "	gcc -Wall -ggdb -rdynamic -lm -ldl `pkg-config --cflags --libs jack` physyn.c -o physyn *.o"
	echo
}

gen_plugin_entry() {
	echo -n "plugins : "
	while [ -n "$1" ]; do
		basename=`echo "$1" | cut -d '.' -f 1`
		echo -n "$basename".so" "
		shift
	done
	echo
	echo
}

echo "all : physyn plugins"
echo

gen_main *.c
gen_plugin_entry */*.c
gen_entries *.c
gen_plugins */*.c

echo "clean :"
echo "	rm *.o *.h physyn"
echo "	rm */*.so */*.o"
