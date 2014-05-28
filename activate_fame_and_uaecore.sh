cp Makefile_fame_and_uaecore.pnd Makefile
cp src/custom_uaecore.cpp src/custom.cpp
cp src/memory_uaecore.cpp src/memory.cpp
cp src/m68k/m68k_intrf_uaecore.h src/m68k/m68k_intrf.h
rm src/*.o
rm src/m68k/*.o
rm src/m68k/fame/m68k_intrf.o
rm src/gp2x/menu/menu_main.o