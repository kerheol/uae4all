cp Makefile_cyclone.pnd Makefile
cp src/custom_cyclone.cpp src/custom.cpp
cp src/memory_cyclone.cpp src/memory.cpp
cp src/m68k/m68k_intrf_cyclone.h src/m68k/m68k_intrf.h
rm src/*.o
rm src/m68k/*.o
rm src/m68k/fame/m68k_intrf.o
rm src/gp2x/menu/menu_main.o