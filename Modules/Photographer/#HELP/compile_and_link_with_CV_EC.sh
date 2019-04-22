#!/bin/bash

cd ~/Documents/Projects/Submarine/Modules/Photographer

#Make static libraries of CV and EthControl
./../CV/#HELP/make_arch.sh
./../EthControl/#HELP/make_arch.sh $1

#Compile main.c with static libraries
cp Code/main.c Code/main_backup_script.c
cat Code/main_backup_script.c | sed "s/\/\/place_for_define/#define $1"/ > Code/main.c
gcc -c Code/main.c -I../CV/Code/Headers -I../EthControl/Code/Headers -I../Headers -o main.o
gcc  main.o -I../CV/Code/Headers -I../EthControl/Code/Headers -I../Headers -L../CV/Archives -lmobsel -L../EthControl/Archives -lethcont -lgd -lpng -lz -ljpeg -lfreetype -lm -o submarine.out

mv Code/main_backup_script.c Code/main.c
rm main.o

exit 0
