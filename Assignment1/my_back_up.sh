#!/bin/bash

if [ $# == 1 ] # $# -> argc
then
	BACKUPPATH=$1
	if [ -d "$BACKUPPATH" ] #isDir
	then
		for i in * # 'ls'
		do
			# isFile or newer then
			if [ ! -f $i ] || [ "$i" -nt "$BACKUPPATH\$i" ]
			then
				echo "MOVE $i to $BACKUPPATH"
				cp -r $i "$BACKUPPATH"
			fi
		done
	else
		mkdir "$BACKUPPATH" && cp -r . "$BACKUPPATH"
	fi
else
	printf "Error. check the argument.\n"
	printf "usage: ./my_back_up.sh backUpFolder\n"
fi
