CONTIKI = ../../../..

TARGET = stm3210e-eval

CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy

CONTIKI_TARGET_MAIN = contiki-main.c
USB_CDC_ETH_CLASS = 1

#UIP_CONF_IPV6 = 0
UIP_CONF_LOGGING = 1
UIP_LOGGING = 1

ifdef UIP_CONF_IPV6
$(error apa)
CFLAGS += -DWITH_UIP6=1
endif

APPS += webserver

all: usbeth


include $(CONTIKI)/Makefile.include


