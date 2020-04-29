#!/bin/bash

rm -f driver_2015_test.cpp
touch driver_2015_test.cpp
#for f in driver_2015.h driver_buffer.h driver_keyboard.h driver_net.h driver_SD.h power_tariffs.h screens.h screens_status.h termometry.h vents.h test.h minunit.h
for f in vents_tst.h vents.h termometry_tst.h termometry.h rest_tst.h rest.h test.h minunit.h driver2015mock.h power_tariffs.h power_tariffs_tst.h driver_buffer_tst.h driver_buffer.h
do
  cat $f | grep -v ".h\""  >> driver_2015_test.cpp
done

#for x in driver_2015.cpp driver_buffer.cpp driver_keyboard.cpp driver_net.cpp driver_SD.cpp power_tariffs.cpp screens.cpp screens_status.cpp termometry.cpp vents.cpp
for x in vents_tst.cpp vents.cpp termometry_tst.cpp termometry.cpp rest_tst.cpp rest.cpp test.cpp driver2015mock.cpp power_tariffs.cpp power_tariffs_tst.cpp driver_buffer_tst.cpp driver_buffer.cpp
do
  cat $x | grep -v ".h\""  >> driver_2015_test.cpp
done

make

./runtests

#rm -f driver_2015_test.cpp
#rm -f runtests

