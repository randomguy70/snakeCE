# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= SNAKE
ICON ?= icon.png
DESCRIPTION ?= "A colorful spinoff of Snake"
COMPRESSED ?= YES
ARCHIVED ?= YES

CFLAGS ?= -Wall -Wextra -O3
# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk

#------------------------------