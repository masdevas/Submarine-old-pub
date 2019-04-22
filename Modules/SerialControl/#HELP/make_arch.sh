#!/bin/bash

# Make objects files
cd ~/Documents/Projects/Submarine/Modules/SerialControl
rm Archives/libsercont.a
for var in $(ls Code | grep -E [.]c$) 
do
	gcc -c "Code/$var" "-I../Headers"
done

# Make archive
ar rc Archives/libsercont.a $(ls | grep -E [.]o$) 
ranlib Archives/libsercont.a

# Remove objects
for var in $(ls | grep -E [.]o$)
do
	rm "$var"
done

exit 0
