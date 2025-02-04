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
	music/pajama_jam.o \
	title/press_start.o \
	title/title.o \
	title/title_bg_tiles_hi.o \
	title/title_bg_tiles_low.o \
	title/title_controller.o \
	title/title_text.o \
	utils/hUGEHelpers.o \
	sprite/naija_sprites.c \
	sfx/cbtfx.o \
	sfx/sfx.o \
	intro/intro_controller.o \
	intro/mia_hug_map.o \
	intro/mia_hug_tiles.o \
	intro/mia_brainwash_map.o \
	intro/mia_brainwash_tiles.o \
	intro/naija_crystal_map.o \
	intro/naija_crystal_tiles.o \
	intro/fishform_gasp_map.o \
	intro/fishform_gasp_tiles.o \
	intro/swordfish_steak_map.o \
	intro/swordfish_steak_tiles.o \
	logos/discord_logo_map.o \
	logos/discord_logo_tiles.o \
	logos/game_by_daxar_logo_map.o \
	logos/game_by_daxar_logo_tiles.o \
	logos/logo_controller.o \
	vvvvvv/vvvvvv_map.o \
	vvvvvv/vvvvvv_map2.o \
	vvvvvv/vvvvvv_mapattribs.o \
	vvvvvv/vvvvvv_mapattribs2.o \
	vvvvvv/vvvvvv_tiles.o \
	vvvvvv/vvvvvv_controller.o


GBDK_DEBUG = ON
# Force MBC5 and force Gameboy Color
LCCFLAGS = -Wl-yt3 -Wl-yo16 -Wl-ya16 -autobank -Wm-yt0x19 -Wm-yoA -Wm-yC -Wm-yn"DERP2" -Wl-llib/hUGEDriver.lib
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
	rm -f main.o main.asm derp.asm derp.lst derp.rst derp.asm derp.adb main.adb main.lst main.adb derp.sym main.sym main.rst **/*.o **/*.lst *.map *.gbc *~ *.rel *.cdb *.ihx *.lnk **/*.sym **/*.asm *.noi **/*.rst **/*.adb
