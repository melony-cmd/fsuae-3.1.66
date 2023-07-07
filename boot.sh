#!/bin/bash
rm /home/inner/Documents/FS-UAE/Cache/Logs/fs-uae.log.txt
make

FILE="/home/inner/Documents/FS-UAE/Save States/AmigaDev/Saved State 1.uss"

./fs-uae /home/inner/Documents/FS-UAE/Configurations/$1


<<'
if test -f "$FILE"; then
  ./fs-uae --load_state=1 /home/inner/Documents/FS-UAE/Configurations/$1
else
  ./fs-uae /home/inner/Documents/FS-UAE/Configurations/$1
fi'


