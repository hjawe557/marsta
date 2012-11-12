CONTIKI = ../../../..

TARGET = u101

CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy

CONTIKI_TARGET_MAIN = contiki-main.c

APPS = serial-shell
APPS += u101-shell

USB_CDC_ETH_CLASS = 1


all: u101-shell

u101-shell_src += u101-shell.c shell-i2c.c shell-spi.c usbeth.c

#UIP_CONF_IPV6 = 1

U101_RF231 = 1

include $(CONTIKI)/Makefile.include

$(warning u101-shell_src $(u101-shell_src))
