#!/bin/bash

# Make objects files
cd ~/Documents/Projects/Submarine/Modules/Server

#Make static libraries of CV and EthControl
./../CV/#HELP/make_arch.sh
./../EthControl/#HELP/make_arch.sh THIS_SERVER
./../SerialControl/#HELP/make_arch.sh

#Compile main.c with static libraries
gcc  Code/main.c -I../CV/Code/Headers -I../EthControl/Code/Headers -I../SerialControl/Code/Headers -I../Headers -L../CV/Archives -lmobsel -lgd -lpng -lz -ljpeg -lfreetype -lm -L../EthControl/Archives -lethcont -L../SerialControl/Archives -lsercont -lserialport -o submarine.out

exit 0
