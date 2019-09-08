#!/bin/bash

echo exporting lib
export LD_LIBRARY_PATH=/home/patrick/Projects/PiProjects/lib

echo "playing..."
./../../bin/music
echo
echo "stopped"