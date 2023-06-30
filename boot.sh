#!/bin/bash
rm /home/inner/Documents/FS-UAE/Cache/Logs/fs-uae.log.txt
make
./fs-uae /home/inner/Documents/FS-UAE/Configurations/$1
