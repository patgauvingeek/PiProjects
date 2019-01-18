#!/bin/bash

echo exporting lib
export LD_LIBRARY_PATH=/home/patrick/Projects/PiProjects/lib

echo exporting path
dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export PATH=$dir/sbin:$PATH

echo Init Database and GPIO
if [ -f pialarm.db ]; then
  rm pialarm.db
fi

echo "Adding sensors"
./../../bin/PiAlarm sensors.add "Main Entrance" door 0
./creategpiofiles.sh 0

./../../bin/PiAlarm sensors.add "Patio" window 1
./creategpiofiles.sh 1

./../../bin/PiAlarm sensors.add "Salon" motion 2
./creategpiofiles.sh 2

./../../bin/PiAlarm sensors.add "Button" button 3
./creategpiofiles.sh 3

./../../bin/PiAlarm sensors.add "RfId" rfid ./tty 9600

echo "Adding notifiers"
./../../bin/PiAlarm notifiers.add "Bell" bell 4
./creategpiofiles.sh 4

./../../bin/PiAlarm notifiers.add "email" message esro.net@gmail.com

echo "Adding users"
echo "" > stty_params
echo "A1B2C3D4E5E1" > ./tty
./../../bin/PiAlarm users.add "Patrick"
actual_stty_params=$(cat stty_params)
expected_stty_params="-F ./tty sane raw pass8 -echo -hupcl clocal 9600"
if [ "$expected_stty_params" != "$actual_stty_params" ]; then
  (>&2 echo "<ERROR> configuring baud rate")
fi
./../../bin/PiAlarm users.add "Josée"
echo "000000000000" > ./tty
./../../bin/PiAlarm users.add "Patrick"
./../../bin/PiAlarm users.add "Josée"
./../../bin/PiAlarm users.delete "Josée"

./../../bin/PiAlarm run &
pid=$!
echo
echo Run as $pid

google-chrome WebSocketTest.html
chrome_pid=$!

sleep 5

./gpioupdateserie.sh

echo Arming the system
echo "A1B2C3D4E5E1" > ./tty
sleep 1
echo "" > ./tty

sleep 20
echo System armed

./gpioupdateserie.sh

echo System expecting unarmed
sleep 20
echo Unarmed never occured

echo Unarming the system
echo "A1B2C3D4E5E1" > ./tty
sleep 1
echo "" > ./tty
sleep 1

echo killing $pid
kill $pid

echo killing chrome
kill $chrome_pid

echo
echo Users
./../../bin/PiAlarm users.list +events

echo
echo Sensors + Events
./../../bin/PiAlarm sensors.list +events

echo
echo Notifiers
./../../bin/PiAlarm notifiers.list

echo
echo Events
./../../bin/PiAlarm events.list
echo
echo Alarms
./../../bin/PiAlarm alarms.list
echo
echo Logs
./../../bin/PiAlarm logs
echo