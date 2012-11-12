TARGET=stm3210e-eval

all: stm3210e


CONTIKI=../..

CONTIKI_TARGET_MAIN=contiki-main.c

PROJECT_SOURCEFILES = parity.c 


randgen: randgen.c
	gcc -DNDEBUG -I $(CONTIKI)/cpu/arm/stm32f103/ -I . -I $(CONTIKI)/core  randgen.c -o randgen

randcheck: randcheck.c
	gcc -DNDEBUG -I $(CONTIKI)/cpu/arm/stm32f103/ -I . -I $(CONTIKI)/core  randcheck.c -o randcheck

clean: stm3210e-eval_clean

.PHONY: stm3210e-eval_clean

stm3210e-eval_clean:

include $(CONTIKI)/Makefile.include

