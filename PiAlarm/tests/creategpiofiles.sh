#!/bin/bash

if [ ! -d gpio ]; then
    mkdir gpio
fi

if [ ! -d gpio/gpio$1 ]; then
    mkdir gpio/gpio$1
fi

echo "$2" > gpio/gpio$1/value
echo "" > gpio/gpio$1/direction


