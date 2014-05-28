// VisualBoyAdvance - Nintendo Gameboy/GameboyAdvance (TM) emulator.
// Copyright (C) 1999-2003 Forgotten
// Copyright (C) 2004 Forgotten and the VBA development team

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or(at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/* 
 * Code adapted To OpenBOR by SX
 * simple2x.c - Trying to scale 2x.
 *
 * Updated: 5/05/08 - SX
 *
 */

#include "scalebit.h"

void Simple2x(unsigned char *srcPtr, unsigned int srcPitch, unsigned char *deltaPtr, unsigned char *dstPtr, unsigned int dstPitch, int width, int height, int scanlines)
{
	unsigned char *nextLine, *finish;
  
	nextLine = dstPtr + dstPitch;
  
	do 
	{
		unsigned int *bP = (unsigned int *) srcPtr;
		unsigned int *dP = (unsigned int *) dstPtr;
		unsigned int *nL = (unsigned int *) nextLine;
		unsigned int currentPixel;
    
		finish = (unsigned char *) bP + ((width+2) << 1);
		currentPixel = *bP++;
    
		do 
		{
#ifdef BIG_ENDIAN
			unsigned int color = currentPixel >> 16;
#else
		    	unsigned int color = currentPixel & 0xffff;
#endif
			unsigned int color2 = 0;

			color = color | (color << 16);
			
			if(scanlines)
			{
				if(scanlines == 1) //25% scanlines
					color2 = ((color & 0xf7de) >> 1) + ((color & 0xe79c) >> 2);
				else //50% scanlines
					color2 = ((color & 0xf7de) >> 1);
			}

			*(dP) = color;
			if(scanlines)
				*(nL) = color2;
			else
				*(nL) = color;

#ifdef BIG_ENDIAN
			color = currentPixel & 0xffff;
#else
			color = currentPixel >> 16;
#endif
			color = color| (color << 16);
			*(dP + 1) = color;
			if(scanlines)
				*(nL + 1) = color2;
			else
				*(nL + 1) = color;

			currentPixel = *bP++;

			dP += 2;
			nL += 2;
		}
		while ((unsigned char *) bP < finish);

		srcPtr += srcPitch;
		dstPtr += dstPitch << 1;
		nextLine += dstPitch << 1;
	}
	while (--height);
}

void Simple2x32(unsigned char *srcPtr, unsigned int srcPitch, unsigned char *deltaPtr, unsigned char *dstPtr, unsigned int dstPitch, int width, int height)
{
    unsigned char *nextLine, *finish;
  
	nextLine = dstPtr + dstPitch;
  
	do 
	{
		unsigned int *bP = (unsigned int *) srcPtr;
	    	unsigned int *dP = (unsigned int *) dstPtr;
		unsigned int *nL = (unsigned int *) nextLine;
		unsigned int currentPixel;
    
		finish = (unsigned char *) bP + ((width+1) << 2);
		currentPixel = *bP++;
    
		do 
		{
			unsigned int color = currentPixel;

		    	*(dP) = color;
			*(dP+1) = color;
			*(nL) = color;
			*(nL + 1) = color;
      
			currentPixel = *bP++;
	      
			dP += 2;
			nL += 2;
		}
		while ((unsigned char *) bP < finish);
    
		srcPtr += srcPitch;
		dstPtr += dstPitch << 1;
		nextLine += dstPitch << 1;
	}
	while (--height);
}
