#!/bin/bash

echo Running door events...
echo "0" > gpio/gpio0/value
sleep 1
echo "1" > gpio/gpio0/value
sleep 1

echo Running window events...
echo "0" > gpio/gpio1/value
sleep 1
echo "1" > gpio/gpio1/value
sleep 1

echo Running motion event...
echo "0" > gpio/gpio2/value
sleep 1
echo "1" > gpio/gpio2/value

sleep 5

echo Arming system
echo "0" > gpio/gpio3/value
sleep 1
echo "1" > gpio/gpio3/value

sleep 20

echo running door alarm
echo "0" > gpio/gpio0/value
sleep 1
echo "1" > gpio/gpio0/value
sleep 16

echo running window alarm
echo "0" > gpio/gpio1/value
sleep 1
echo "1" > gpio/gpio1/value
sleep 1

echo running motion alarm
echo "0" > gpio/gpio2/value
sleep 1
echo "1" > gpio/gpio2/value

echo Unarming system
echo "0" > gpio/gpio3/value
sleep 1
echo "1" > gpio/gpio3/value
