Cortex M3 is a CPU specification and IP core developed by ARM 
implementing the ARM v7-M architecture. Hardware vendors license 
the CPU IP and implement actual silicon. 

The Cortex M3 CPU specification is available at:
http://infocenter.arm.com/help/topic/com.arm.doc.subset.cortexm.m3

This directory contains stuff common for ARM Cortex M3 CPUs, which
more or less means the System Control Space (SCS), Nested Vectored
Interrupt Controller (NVIC) and SysTick.

ARM and other vendors provide a software library that incorporates
all of this - the CMSIS library. However, it is unclear if the
license of CMSIS is compatible with the BSD license.
