CONTIKI = ../..
ifndef TARGET
TARGET=sky
endif

all: blink sky-collect #rt-leds test-button test-cfs tcprudolph0 Vaisala reading_level test Pulse_Sensors isr rssi-scanner client basestation sensornode azmayesh Pulse_Sensors_Correction
CFLAGS+=-DRF_CHANNEL=15
%.tgz: %.ihex
	mkdir $(basename $<) ; \
	mv $< $(basename $<) ; \
	echo $(basename $<)/$(basename $<).ihex 600 > $(basename $<)/runfile ; \
	tar czf $@ $(basename $<)

%.class: %.java
	javac $(basename $<).java

viewrssi: ViewRSSI.class
	make login | java ViewRSSI

include $(CONTIKI)/Makefile.include
