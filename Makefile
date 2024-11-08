# If you move this project you can change the directory
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
ifndef GBDK_HOME
	GBDK_HOME = ../gbdk/
endif

LCC = $(GBDK_HOME)bin/lcc -Wa-l -Wl-m -Wl-j -I./include

BINS	= derp.gbc

OBJS	= derp.o \
	music/pressure_cooker.o \
	music/space_odyssey.o \
	music/the_traveller.o \
	title/title_bg_tiles_hi.o \
	title/title_bg_tiles_low.o \
	title/title_text.o \
	title/title.o \
	title/press_start.o \
	font/font_tiles.o \
	title/title_controller.o \
	utils/hUGEHelpers.o \

GBDK_DEBUG = ON
# Force MBC1 and force Gameboy Color
LCCFLAGS = -Wm-yt0x01 -Wm-yC -Wm-yn"DERP2" -Wl-llib/hUGEDriver.lib
ifdef GBDK_DEBUG
	LCCFLAGS += -debug -v
endif

all:	$(BINS)

# Compile and link single file in one pass
%.gbc:	$(OBJS)
	$(LCC) $(LCCFLAGS) -o $@ $(OBJS)

%.o:	%.c
	$(LCC) -debug -v -c -o $@ $<

clean:
	rm -f *.o **/*.lst *.map *.gbc *~ *.rel *.cdb *.ihx *.lnk **/*.sym **/*.asm *.noi **/*.rst **/*.adb
