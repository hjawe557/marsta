TARGET=u101

all: acctest


CONTIKI=../..

CONTIKI_TARGET_MAIN = contiki-main.c

PROJECT_SOURCEFILES = parity.c 


randgen: randgen.c
	gcc -DNDEBUG -I $(CONTIKI)/cpu/arm/stm32f103/ -I . -I $(CONTIKI)/core  randgen.c -o randgen

randcheck: randcheck.c
	gcc -DNDEBUG -I $(CONTIKI)/cpu/arm/stm32f103/ -I . -I $(CONTIKI)/core  randcheck.c -o randcheck

clean: u101_clean

.PHONY: u101_clean

u101_clean:

include $(CONTIKI)/Makefile.include

