PREFIX	=/usr

CROSS_COMPILE	= /opt/gcw0-toolchain/usr/bin/mipsel-linux-
SYSROOT			= $(shell $(CC) --print-sysroot)

MORE_CFLAGS += -DGP2X -DDINGOO -DGCW0 -DDOUBLEBUFFER -I$(PREFIX)/include/ -DNDEBUG
#MORE_CFLAGS += -DSOFTSTRETCH

NAME	= uae4all
O		= o
RM		= rm -f
CC		= $(CROSS_COMPILE)gcc
CXX		= $(CROSS_COMPILE)g++
STRIP	= $(CROSS_COMPILE)strip
AS		= $(CROSS_COMPILE)as

PROG   = $(NAME)

all: $(PROG)

#CYCLONE_CORE=1
ifndef CYCLONE_CORE
FAME_CORE=1
FAME_CORE_C=1
endif

DEFAULT_CFLAGS = $(shell $(SYSROOT)/usr/bin/sdl-config --cflags)
#LDFLAGS        = `$(SDL_BASE)sdl-config --libs` -lSDL_mixer -lsmpeg -lstdc++ -lSDL -lmikmod -lm -lz -static
#LDFLAGS        =  `$(SDL_BASE)sdl-config --libs` -lm -lSDL_gfx  -static -lSDL -ljpeg -lpng12 -lz -lSDL_mixer -lmikmod -lvorbisidec -lmad -lSDL \
-lpthread -lgcc -lc -ldl -msoft-float -lm
#`$(SDL_BASE)sdl-config --libs` -lSDL -lz -static -lSDL_gfx
LDFLAGS = -L$(SYSROOT)/usr/include/lib -Wl,-rpath,$(SYSROOT)/usr/include/lib -lSDL  -lpthread  -lz -lSDL_image -lpng14 $(shell $(SYSROOT)/usr/bin/sdl-config --libs) -v

#MORE_CFLAGS += -DGP2X
#-Wall -fprofile-use
# -lSDL_mixer -lSDL_image
MORE_CFLAGS +=   -I"$(SYSROOT)/usr/include/"   -I"$(SYSROOT)/usr/include/SDL" -Isrc -Isrc/gp2x -Isrc/menu -Isrc/include -Isrc/gp2x/menu -Isrc/vkbd -fomit-frame-pointer -Wno-unused -Wno-format -DUSE_SDL -DGCCCONSTFUNC="__attribute__((const))" -DUSE_UNDERSCORE -fno-exceptions -DUNALIGNED_PROFITABLE -DOPTIMIZED_FLAGS -DSHM_SUPPORT_LINKS=0 -DOS_WITHOUT_MEMORY_MANAGEMENT -DVKBD_ALWAYS
#-DMENU_MUSIC

MORE_CFLAGS += -DROM_PATH_PREFIX=\"./\" -DDATA_PREFIX=\"./data/\" -DSAVE_PREFIX=\"./saves/\"
//MORE_CFLAGS += -DROM_PATH_PREFIX=\"./\" -DDATA_PREFIX=\"./data/\" -DSAVE_PREFIX=\"/home/john4p/\"

MORE_CFLAGS += -msoft-float -ffast-math
ifndef DEBUG
MORE_CFLAGS += -O3
MORE_CFLAGS += -fstrict-aliasing -fexpensive-optimizations
MORE_CFLAGS += -fweb -frename-registers -fomit-frame-pointer
MORE_CFLAGS += -falign-functions=32 -falign-loops -falign-labels -falign-jumps
MORE_CFLAGS += -finline -finline-functions -fno-builtin
else
MORE_CFLAGS += -ggdb
endif

ASFLAGS += -msoft-float

#MORE_CFLAGS+= -DUSE_AUTOCONFIG
#MORE_CFLAGS+= -DUAE_CONSOLE

MORE_CFLAGS+= -DUSE_ZFILE
#MORE_CFLAGS+= -DDEBUG_SAVESTATE

#MORE_CFLAGS+= -DUAE4ALL_NO_USE_RESTRICT

MORE_CFLAGS+= -DNO_SOUND
#MORE_CFLAGS+= -DNO_THREADS

#MORE_CFLAGS+= -DDEBUG_TIMESLICE

MORE_CFLAGS+= -DFAME_INTERRUPTS_PATCH
#MORE_CFLAGS+= -DFAME_INTERRUPTS_SECURE_PATCH

# Turrican3 becomes unstable if this is not enabled
MORE_CFLAGS+= -DSAFE_MEMORY_ACCESS

#MORE_CFLAGS+= -DUAE_MEMORY_ACCESS
#MORE_CFLAGS+= -DERROR_WHEN_MEMORY_OVERRUN

MORE_CFLAGS+= -DDEBUG_UAE4ALL
#MORE_CFLAGS+= -DDEBUG_UAE4ALL_FFLUSH
#MORE_CFLAGS+= -DDEBUG_M68K
#MORE_CFLAGS+= -DDEBUG_INTERRUPTS
#MORE_CFLAGS+= -DDEBUG_CIA
#MORE_CFLAGS+= -DDEBUG_SOUND
#MORE_CFLAGS+= -DDEBUG_MEMORY
#MORE_CFLAGS+= -DDEBUG_MAPPINGS
#ORE_CFLAGS+= -DDEBUG_DISK
#ORE_CFLAGS+= -DDEBUG_CUSTOM
#MORE_CFLAGS+= -DDEBUG_EVENTS
#MORE_CFLAGS+= -DDEBUG_GFX -DDEBUG_BLITTER
#MORE_CFLAGS+= -DDEBUG_FRAMERATE
##MORE_CFLAGS+= -DAUTO_FRAMERATE=1400
##MORE_CFLAGS+= -DMAX_AUTO_FRAMERATE=4400
##MORE_CFLAGS+= -DAUTO_FRAMERATE_SOUND
#MORE_CFLAGS+= -DSTART_DEBUG=11554
#MORE_CFLAGS+= -DMAX_AUTOEVENTS=11560
#MORE_CFLAGS+= -DAUTO_RUN


#MORE_CFLAGS+= -DPROFILER_UAE4ALL

CFLAGS  = $(DEFAULT_CFLAGS) $(MORE_CFLAGS)

OBJS =	\
	src/custom.o \
	src/audio.o \
	src/autoconf.o \
	src/blitfunc.o \
	src/blittable.o \
	src/blitter.o \
	src/cia.o \
	src/savedisk.o \
	src/savestate.o \
	src/compiler.o \
	src/disk.o \
	src/drawing.o \
	src/ersatz.o \
	src/gfxutil.o \
	src/keybuf.o \
	src/main.o \
	src/md-support.o \
	src/memory.o \
	src/missing.o \
	src/gui.o \
	src/od-joy.o \
	src/sound_gp2x.o \
	src/sdlgfx.o \
	src/writelog.o \
	src/zfile.o \
	src/menu/fade.o \
	src/vkbd/vkbd.o \
	src/gp2x/gp2x.o \
	src/gp2x/inputmode.o \
	src/gp2x/menu/menu.o \
	src/gp2x/menu/menu_fileinfo.o \
	src/gp2x/menu/menu_load.o \
	src/gp2x/menu/menu_main.o \
	src/gp2x/menu/menu_savestates.o \
	src/gp2x/menu/menu_misc.o \
	src/gp2x/menu/menu_controls.o \
	src/gp2x/menu/menu_display.o \
	src/scale2x/getopt.o \
	src/scale2x/pixel.o \
	src/scale2x/scale2x.o \
	src/scale2x/scale3x.o \
	src/scale2x/scalebit.o \
	src/scale2x/simple2x.o \

	# src/gp2x/memcpy.o \
	# src/gp2x/memset.o \
	# src/gp2x/menu/asmutils.o \

ifdef FAME_CORE
ifdef FAME_CORE_C
#CFLAGS+=-DUSE_FAME_CORE -DUSE_FAME_CORE_C -DFAME_INLINE_LOOP -DFAME_IRQ_CLOCKING -DFAME_CHECK_BRANCHES -DFAME_EMULATE_TRACE -DFAME_DIRECT_MAPPING -DFAME_BYPASS_TAS_WRITEBACK -DFAME_ACCURATE_TIMING -DFAME_GLOBAL_CONTEXT -DFAME_FETCHBITS=8 -DFAME_DATABITS=8 -DFAME_GOTOS -DFAME_EXTRA_INLINE=__inline__ -DFAME_NO_RESTORE_PC_MASKED_BITS
CFLAGS+=-DUSE_FAME_CORE -DUSE_FAME_CORE_C -DFAME_IRQ_CLOCKING -DFAME_CHECK_BRANCHES -DFAME_EMULATE_TRACE -DFAME_DIRECT_MAPPING -DFAME_BYPASS_TAS_WRITEBACK -DFAME_ACCURATE_TIMING -DFAME_GLOBAL_CONTEXT -DFAME_FETCHBITS=8 -DFAME_DATABITS=8 -DFAME_NO_RESTORE_PC_MASKED_BITS
src/m68k/fame/famec.o: src/m68k/fame/famec.cpp
OBJS += src/m68k/fame/famec.o
else
CFLAGS+=-DUSE_FAME_CORE
src/m68k/fame/fame.o: src/m68k/fame/fame.asm
	nasm -f elf src/m68k/fame/fame.asm
OBJS += src/m68k/fame/fame.o
endif
OBJS += src/m68k/fame/m68k_intrf.o
else
OBJS += \
	src/m68k/uae/newcpu.o \
	src/m68k/uae/readcpu.o \
	src/m68k/uae/cpudefs.o \
	src/m68k/uae/fpp.o \
	src/m68k/uae/cpustbl.o \
	src/m68k/uae/cpuemu.o
endif



CPPFLAGS  = $(CFLAGS)

$(PROG): $(OBJS)
	$(CXX) $(CFLAGS) -o $(PROG) $(OBJS) $(LDFLAGS)
ifndef DEBUG
	$(STRIP) $(PROG)
endif


run: $(PROG)
	./$(PROG)

clean:
	$(RM) $(PROG) $(OBJS)

up: $(PROG)
	cp -v $< /mnt/gp2x/mnt/sd/emus/uae4all/

