CONTIKI = ../../../..

TARGET = u101

CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy

CONTIKI_TARGET_MAIN = contiki-main.c

#UIP_CONF_IPV6 = 1
USB_CDC_ETH_CLASS = 1
U101_RF231 = 1
CFLAGS += -DWITH_UIP=1
CFLAGS += -DWITH_USB=1
UIP_CONF_LOGGING = 1
UIP_LOGGING = 1

APPS += webserver

all: usbeth


include $(CONTIKI)/Makefile.include
