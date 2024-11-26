# If you move this project you can change the directory
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
ifndef GBDK_HOME
	GBDK_HOME = ../gbdk/
endif

LCC = $(GBDK_HOME)bin/lcc -Wa-l -Wl-m -Wl-j -I./include

BINS	= derp.gbc

OBJS	= main.o \
	aquaria/aquaria_controller.o \
	aquaria/map1_tiles.o \
	aquaria/map1_tiles2.o \
	aquaria/tileset.o \
	font/font_tiles.o \
	music/pressure_cooker.o \
	music/space_odyssey.o \
	music/the_traveller.o \
	title/press_start.o \
	title/title.o \
	title/title_bg_tiles_hi.o \
	title/title_bg_tiles_low.o \
	title/title_controller.o \
	title/title_text.o \
	utils/hUGEHelpers.o


GBDK_DEBUG = ON
# Force MBC1 and force Gameboy Color
LCCFLAGS = -Wl-yt3 -Wl-yo4 -Wl-ya4 -autobank -Wm-yt0x19 -Wm-yoA -Wm-yC -Wm-yn"DERP2" -Wl-llib/hUGEDriver.lib
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
