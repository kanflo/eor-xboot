#!/bin/make

PROGRAM=xboot
OTA=1
EXTRA_COMPONENTS = extras/rboot-ota extras/dhcpserver extras/cli
include $(EOR_ROOT)/common.mk
