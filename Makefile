# ------------------------------------------------
# Generic Makefile
#
# Author:	Silviu Tantos
# Email	:	me@razius.com
# Date  :	2012-08-10
#
# Changelog :
#   2012-08-10 - First version.
#   2013-02-16 - Added wildcards for sources.
#   2013-05-11 - Added .. for doxygen documentation.
# ------------------------------------------------

# Variables
CMCU=atmega1280
PMCU=m1280
TARGET=M1280

# Paths
CC = `which avr-gcc`
PC = `which avrdude`

# Directories
SDIR = src
BDIR = bin
ODIR = obj
LDIR = lib

# Files
SOURCES := $(shell find $(SDIR) -name '*.c')
# SOURCES = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(wildcard $(SDIR)/*.c))

# Flags
CFLAGS=-DF_CPU=7372800L -std=gnu99 -mmcu=$(CMCU) -I$(SDIR) -Wall -pedantic -Os -g
LFLAGS = -Wall -I. -lm
PFLAGS=-p $(PMCU) -c usbasp -U

build: clean program

program: $(BDIR)/$(TARGET).hex
	$(PC) $(PFLAGS) flash:w:$(BDIR)/$(TARGET).hex

clean:
	rm -rf $(ODIR)/* $(BDIR)/*

docs:
	@echo "Here"

all: clean build docs program

$(ODIR)/$(TARGET).obj:
	$(CC) $(CFLAGS) $(SOURCES) -o $@

$(BDIR)/$(TARGET).hex: $(ODIR)/$(TARGET).obj
	avr-strip $<
	avr-objcopy -O ihex -R .eeprom $< $@

.PHONY: build program clean docs all
