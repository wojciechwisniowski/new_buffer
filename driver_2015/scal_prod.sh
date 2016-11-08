#!/bin/bash

rm -f driver_2015.ino
touch driver_2015.ino
for f in driver_2015.h driver_buffer.h driver_keyboard.h driver_net.h driver_SD.h power_tariffs.h screens.h screens_status.h termometry.h termometry_prod.h vents.h vents_prod.h prod.h
do
  cat $f | grep -v ".h\""  >> driver_2015.ino
done

for x in driver_2015.cpp driver_buffer.cpp driver_keyboard.cpp driver_net.cpp driver_SD.cpp power_tariffs.cpp screens.cpp screens_status.cpp termometry.cpp termometry_prod.cpp vents.cpp vents_prod.cpp
do
  cat $x | grep -v ".h\""  >> driver_2015.ino
done

cp driver_2015.ino /root/sketchbook/driver_2015/

rm -f driver_2015.ino