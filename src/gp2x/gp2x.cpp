#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <stdlib.h>
#include <linux/soundcard.h>
#include "SDL.h"
#include "gp2x.h"
#include "gp2xutil.h"

#include "menu/menu.h"

extern int uae4all_keystate[256];
extern void record_key(int);

int vol = 100;
unsigned long memDev;
#ifdef WIZ
int stickfd=0;
#else
#ifdef PANDORA
FILE* fPndButtonMode;
extern SDL_Joystick *uae4all_joy0;
#else
volatile unsigned short* MEM_REG;
#endif
#endif
int gp2xClockSpeed;


static int mixerdev;
int soundVolume = 50;
int flashLED;

int gp2xMouseEmuOn=0;
int gp2xButtonRemappingOn=0;
int hasGp2xButtonRemapping=1;
int enableHWscaling=DEFAULT_SCALING;
int GFXVIDINFO_HEIGHT=240;
char launchDir[300];
char currentDir[300];

extern int graphics_init (void);

//#define SOUND_MIXER_WRITE_PCM 0xc0044d04

#define GPIOHOUT 0x106E
#define GPIOHPINLVL 0x118E

// TODO ker ver que es esto del mmuhack
#if !defined(PANDORA) && !defined(GCW0)
static int mmuhack(int first_time)
{
	int mmufd;
	char cmdLine[256];

	if (first_time)
	{
		// unload possibly wrong mmuhack if it's loaded
		system("/sbin/rmmod mmuhack");
		snprintf(cmdLine, sizeof(cmdLine), "/sbin/insmod -f %s/mmuhack.o", launchDir);
		system(cmdLine);
	}

	mmufd = open("/dev/mmuhack", O_RDWR);
	if (mmufd < 0)
	{
		return 1;
	}

	close(mmufd);

	return 0;
}
#endif

unsigned long gp2x_joystick_read(int allow_usb_joy)
{
	unsigned long value = 0;

	SDL_Event event;

#ifdef GCW0

while(SDL_PollEvent(&event))
{

	switch(event.type)
	{
		case SDL_QUIT:
		break;
		case SDL_KEYDOWN:			// Button press
			switch(event.key.keysym.sym)
			{
				case SDLK_LEFT:		// D-PAD LEFT
					value |= GP2X_LEFT;
				break;
				case SDLK_RIGHT:	// D-PAD RIGHT
					value |= GP2X_RIGHT;
				break;
				case SDLK_UP:		// D-PAD UP
					value |= GP2X_UP;
				break;
				case SDLK_DOWN:		// D-PAD DOWN
					value |= GP2X_DOWN;
				break;
				case SDLK_LCTRL:	// A
					value |= GP2X_A;
				break;
				case SDLK_LALT:		// B
					value |= GP2X_B;
				break;
				case SDLK_LSHIFT:	// X
					value |= GP2X_X;
				break;
				case SDLK_SPACE:	// Y
					value |= GP2X_Y;
				break;
				case SDLK_TAB:		// Left shoulder
					value |= GP2X_L;
				break;
				case SDLK_BACKSPACE:	// Right shoulder
					value |= GP2X_R;
				break;
				case SDLK_RETURN:	// Start
					value |= GP2X_START;
				break;
				case SDLK_ESCAPE:	// Select
					value |= GP2X_SELECT;
				break;
			}
		break;
		case SDL_JOYAXISMOTION:			// Analog joystick movement
			switch(event.jaxis.axis)
			{
				case 0:		// axis 0 (left-right)
					if(event.jaxis.value < -JOY_DEADZONE)
					{
						// left movement
						value |= GP2X_LEFT;
					}
					else if(event.jaxis.value > JOY_DEADZONE)
					{
						// right movement
						value |= GP2X_RIGHT;
					}
				break;
				case 1:		// axis 1 (up-down)
					if(event.jaxis.value < -JOY_DEADZONE)
					{
						// up movement
						value |= GP2X_UP;
					}
					else if(event.jaxis.value > JOY_DEADZONE)
					{
						// down movement
						value |= GP2X_DOWN;
					}
				break;

				default:
				break;
			}
		break;

		default:
		break;
	}
}

#else

#ifdef WIZ
	read(stickfd, &value, 4);
#else
#ifdef PANDORA
	char u,d,l,r,ul,ur,dl,dr;

	SDL_JoystickUpdate();
	r  = (SDL_JoystickGetAxis(uae4all_joy0, 0) > 0) ? 1 : 0;
	l  = (SDL_JoystickGetAxis(uae4all_joy0, 0) < 0) ? 1 : 0;
	u  = (SDL_JoystickGetAxis(uae4all_joy0, 1) < 0) ? 1 : 0;
	d  = (SDL_JoystickGetAxis(uae4all_joy0, 1) > 0) ? 1 : 0;
	ul = (u && l) ? 1 : 0;
	ur = (u && r) ? 1 : 0;
	dl = (d && l) ? 1 : 0;
	dr = (d && r) ? 1 : 0;

	if (r)  value |= GP2X_RIGHT;
	if (l)  value |= GP2X_LEFT;
	if (u)  value |= GP2X_UP;
	if (d)  value |= GP2X_DOWN;
	if (ul) value |= GP2X_UP_LEFT;
	if (ur) value |= GP2X_UP_RIGHT;
	if (dl) value |= GP2X_DOWN_LEFT;
	if (dr) value |= GP2X_DOWN_RIGHT;

	if (SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_A))	     value |= GP2X_A;
	if (SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_X))	     value |= GP2X_X;
	if (SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_Y))	     value |= GP2X_Y;
	if (SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_B))	     value |= GP2X_B;
	if (SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_L))	     value |= GP2X_L;
	if (SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_R))	     value |= GP2X_R;
	if (SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_START))  value |= GP2X_START;
	if (SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_SELECT)) value |= GP2X_SELECT;
	if (SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_CLICK))  value |= GP2X_PUSH;
#else // GP2X
	int i;
	value=(MEM_REG[0x1198>>1] & 0x00FF);
	if(value==0xFD) value=0xFA;
	if(value==0xF7) value=0xEB;
  	if(value==0xDF) value=0xAF;
  	if(value==0x7F) value=0xBE;
	value = ~((MEM_REG[0x1184>>1] & 0xFF00) | value | (MEM_REG[0x1186>>1] << 16));
#endif
#endif

#endif
	return value;
}


void gp2x_init(int argc, char **argv)
{
	parse_cmdline(argc, argv);
	unsigned long memdev;
	gp2xClockSpeed = -1;

#if !defined(GCW0)

#if !defined(PANDORA)
#ifdef WIZ
	stickfd = open("/dev/GPIO", O_RDWR | O_NDELAY );
	if(stickfd < 0){
	  printf( "GPIO OPEN FAIL\n");
	} else {
	  puts("keyboard emulation started");
	}
#else	// GP2X
	memDev = open("/dev/mem", O_RDWR);
	MEM_REG=(unsigned short *)mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED,memDev, 0xc0000000);
#endif
#endif
	mixerdev = open("/dev/mixer", O_RDWR);

#if !defined(PANDORA)
	soundVolume=50;
	gp2x_set_volume(soundVolume);
	flashLED = 0;
#else
	fPndButtonMode = fopen( "/proc/pandora/game_button_mode", "r+" ); // 1 = keyboard 2 = gamepad
#endif

#endif

	SDL_ShowCursor(SDL_DISABLE);
	getcwd(launchDir, 250);
	getcwd(currentDir, 250);
	strcat(currentDir,"/roms/");
}

void gp2x_close( void )
{
#if defined(WIZ)
	close(stickfd);
#endif
}

int is_overridden_button(int button)
{
	// TODO: load from file
	return button == GP2X_BUTTON_L || button == GP2X_BUTTON_R ||
		   button == GP2X_BUTTON_A || button == GP2X_BUTTON_B ||
		   button == GP2X_BUTTON_X || button == GP2X_BUTTON_Y;
}

int get_key_for_button(int button)
{
	// TODO: load from file
	/*if (button == GP2X_BUTTON_L)
	{
		return 0x66; // left amiga
	}
	else if (button == GP2X_BUTTON_R)
	{
		return 0x67; // right amiga
	}
	else if (button == GP2X_BUTTON_A)
	{
		return 0x50; // f1
	}
	else if (button == GP2X_BUTTON_B)
	{
		return 0x4d; // down arrow
	}
	else if (button == GP2X_BUTTON_X)
	{
		return 0x40; // space
	}
	else if (button == GP2X_BUTTON_Y)
	{
		return 0x40; // space
	}*/
	return 0;
}

// apply the remapped button keystroke
void handle_remapped_button_down(int button)
{
	int key = get_key_for_button(button);
	if (!uae4all_keystate[key])
	{
		uae4all_keystate[key] = 1;
		record_key(key << 1);
	}
}

void handle_remapped_button_up(int button)
{
	int key = get_key_for_button(button);

	if (uae4all_keystate[key])
	{
		uae4all_keystate[key] = 0;
		record_key((key << 1) | 1);
	}
}

#if !defined(PANDORA)
void gp2x_set_volume(int volume)
{
#if !defined(GCW0)
	int vol = (((volume*0x50)/100)<<8)|((volume*0x50)/100);
	ioctl(mixerdev, SOUND_MIXER_WRITE_PCM, &vol);
#endif
}

void setBatteryLED(int state)
{
	/*if (flashLED)
	{
		int i;

		// On/Off battery LED
		if(MEM_REG[GPIOHPINLVL >> 1] & 16)
			i = 0;
		else
			i = 1;

		if (i!=state)
		{
			MEM_REG[GPIOHOUT >> 1] ^= 16;
		}
	}*/
}
#endif

void switch_to_hw_sdl(int first_time)
{
	// unmap memory, because we don't wat it to be mmuhacked
#if !defined(PANDORA) && !defined(GCW0)
#ifndef WIZ
	munmap((void *)MEM_REG, 0x10000);
#endif
	printf("switching to SDL_HWSURFACE... "); fflush(stdout);
	// 328, 256;
	if (enableHWscaling){
		prSDLScreen = SDL_SetVideoMode(328,256,16,SDL_HWSURFACE|SDL_FULLSCREEN);
		GFXVIDINFO_HEIGHT=256;
	}
	else {
		prSDLScreen = SDL_SetVideoMode(320,240,16,SDL_HWSURFACE|SDL_FULLSCREEN);
		GFXVIDINFO_HEIGHT=240;
	}
#else
	GFXVIDINFO_HEIGHT=240;
#endif
	SDL_ShowCursor(SDL_DISABLE);
	printf(prSDLScreen ? "done\n" : "failed\n");
	usleep(100*1000);
#if !defined(PANDORA) && !defined(GCW0)
	// TODO ker: ver que es esto del mmuhack
	mmuhack(first_time);

	// map again
#ifndef WIZ
	MEM_REG=(unsigned short *)mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED,memDev, 0xc0000000);
#endif
#endif
	// reinit video
	graphics_init();
}

void switch_to_sw_sdl(void)
{
#if !defined(PANDORA) && !defined(GCW0)
	printf("switching to SDL_SWSURFACE... "); fflush(stdout);
	prSDLScreen = SDL_SetVideoMode(320,240,16,SDL_SWSURFACE|SDL_FULLSCREEN);
	SDL_ShowCursor(SDL_DISABLE);
	printf(prSDLScreen ? "done\n" : "failed\n");
#endif
}

