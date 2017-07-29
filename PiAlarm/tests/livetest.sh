#!/bin/bash

echo exporting lib
export LD_LIBRARY_PATH=/home/patrick/Projects/PiProjects/lib


echo Init Database and GPIO
if [ -f pialarm.db ]; then
  rm pialarm.db
fi

./../../bin/PiAlarm sensors.add "Main Entrance" 0 door
./creategpiofiles.sh 0

./../../bin/PiAlarm sensors.add "Patio" 1 window
./creategpiofiles.sh 1

./../../bin/PiAlarm sensors.add "Salon" 2 motion
./creategpiofiles.sh 2

./../../bin/PiAlarm sensors.add "Button" 3 button
./creategpiofiles.sh 3

./../../bin/PiAlarm run &
pid=$!
echo Run as $pid

sleep 5

./gpioupdateserie.sh
sleep 1
./gpioupdateserie.sh

sleep 1
echo killing $pid
kill $pid

echo
echo Sensors + Events
./../../bin/PiAlarm sensors.list +events
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
