SOURCES = driver_2015_eclipse/rest.cpp \
          driver_2015_eclipse/driver2015mock.cpp \
          driver_2015_eclipse/MyPrint.cpp

OBJECTS := $(addsuffix .o, $(addprefix .build/, $(basename $(SOURCES))))
DEPFILES := $(subst .o,.dep, $(subst .build/,.deps/, $(OBJECTS)))
TESTCPPFLAGS = -D_TEST_ -Ilibraries/dsm2_tx -Itest -Iarduino 
#-I/home/src/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.14/cores/arduino 
CPPDEPFLAGS = -MMD -MP -MF .deps/$(basename $<).dep
RUNTEST := $(if $(COMSPEC), a.out, runtest)

all: clean runtests

.build/%.o: %.cpp
	mkdir -p .deps/$(dir $<)
	mkdir -p .build/$(dir $<)
	$(COMPILE.cpp) $(TESTCPPFLAGS) $(CPPDEPFLAGS) -o $@ $<

runtests: $(OBJECTS)
	$(CC) $(OBJECTS) -lstdc++ -o $@
	
#a.out:
#	g++ $(OBJECTS) -ansi	

clean:
	@rm -rf .deps/ .build/ runtest

-include $(DEPFILES)