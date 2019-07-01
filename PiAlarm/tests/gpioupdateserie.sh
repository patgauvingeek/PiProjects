#!/bin/bash

echo Running door events...
echo "1" > gpio/gpio0/value
sleep 1
echo "0" > gpio/gpio0/value
sleep 1

echo Running window events...
echo "1" > gpio/gpio1/value
sleep 1
echo "0" > gpio/gpio1/value
sleep 1

echo Running motion event...
echo "1" > gpio/gpio2/value
sleep 1
echo "0" > gpio/gpio2/value
sleep 1

echo Running button event...
echo "0" > gpio/gpio3/value
sleep 1
echo "1" > gpio/gpio3/value
sleep 1