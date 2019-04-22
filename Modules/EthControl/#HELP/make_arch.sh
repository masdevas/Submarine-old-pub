#!/bin/bash
cd ~/Documents/Projects/Submarine/Modules/EthControl

#Backup .h and replace #define host by arguments command line
cp Code/Headers/eth_control.h Code/Headers/eth_control_backup_of_make_arch.h
cat Code/Headers/eth_control_backup_of_make_arch.h | sed "s/\/\/place_for_define/#define $1"/ > Code/Headers/eth_control.h

rm Archives/libethcont.a
# Make objects files
for var in $(ls Code | grep -E [.]c$)
do
	gcc -c "Code/$var" "-I../CV/Code/Headers" "-I../Headers"
done

# Make archive
ar rc Archives/libethcont.a $(ls | grep -E [.]o$)

# Remove objects
for var in $(ls | grep -E [.]o$)
do
	rm "$var"
done

mv Code/Headers/eth_control_backup_of_make_arch.h Code/Headers/eth_control.h

exit 0
