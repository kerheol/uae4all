#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "menu.h"

#include <sys/stat.h>
#include <unistd.h>
#include<dirent.h>

#include "sysconfig.h"
#include "sysdeps.h"
#include "uae.h"
#include "options.h"
#include "sound.h"
#include <SDL.h>
#include "gp2x.h"
#include <SDL_ttf.h>
static char *text_str_status_line="Status ln";

char *text_str_misc_separator="----------------------------------";
char *text_str_misc_title=    "            Miscellanous         -";
static char *text_str_sndrate="Sound rate";
static char *text_str_44k="44k";
static char *text_str_32k="32k";
static char *text_str_22k="22k";
static char *text_str_11k="11k";
static char *text_str_8k="8k";
static char *text_str_stylus_offset="StylusOffset";
static char *text_str_0px="0px";
static char *text_str_1px="1px";
static char *text_str_3px="3px";
static char *text_str_5px="5px";
static char *text_str_8px="8px";
static char *text_str_tap_delay="Tap delay";
static char *text_str_normal="normal";
static char *text_str_short="short";
static char *text_str_none="no";
static char *text_str_mouse_multiplier="Mouse speed";
static char *text_str_025x=".25";
static char *text_str_05x=".5";
static char *text_str_1x="1x";
static char *text_str_2x="2x";
static char *text_str_4x="4x";
int menuMisc = 0;
//extern int mainMenu_ntsc;
extern int mainMenu_useExternalDrive;
//extern int mainMenu_useOneMegaChip;
extern int mainMenu_chipMemory;
extern int mainMenu_bogoMemory;
extern int mainMenu_joyConf;
extern int mainMenu_scanlines;
extern int mainMenu_autosave;
extern int mainMenu_autofire;
extern int mainMenu_showStatus;
extern int skipintro;
extern int enableHWscaling;

extern int mainMenu_enableScreenshots;
extern int mainMenu_enableScripts;
//extern int mainMenu_frameskip;
extern int mainMenu_mouseMultiplier;
extern int mainMenu_stylusOffset;
extern int mainMenu_tapDelay;
extern int mainMenu_cpuSpeed;
extern int nr_drives;
extern int kickstart;
extern int sound_rate;
extern SDL_Joystick *uae4all_joy0;

char cpuSpeed[5];

static void draw_miscMenu(int c)
{
	int leftMargin=3;
	int tabstop1 = 17;
	int tabstop2 = 19;
	int tabstop3 = 21;
	int tabstop4 = 23;
	int tabstop5 = 25;
	int tabstop6 = 27;
	int tabstop7 = 29;
	int tabstop8 = 31;
	int tabstop9 = 33;

	int menuLine = 3;
	static int b=0;
	int bb=(b%6)/3;
	SDL_Rect r;
	extern SDL_Surface *text_screen;
	r.x=80-64; r.y=0; r.w=110+64+64; r.h=240;

	text_draw_background();
	text_draw_window(2,2,40,30,text_str_misc_title);

	// 1
	if (menuMisc == 0 && bb)
		write_text_inv(3, menuLine, "Return to main menu");
	else
		write_text(3, menuLine, "Return to main menu");

	menuLine++;
	write_text(leftMargin,menuLine,text_str_misc_separator);
	menuLine++;

	// 2
	write_text(leftMargin,menuLine,text_str_sndrate);

	if ((sound_rate==8000)&&((menuMisc!=1)||(bb)))
		write_text_inv(tabstop1,menuLine,text_str_8k);
	else
		write_text(tabstop1,menuLine,text_str_8k);

	if ((sound_rate==11025)&&((menuMisc!=1)||(bb)))
		write_text_inv(tabstop3,menuLine,text_str_11k);
	else
		write_text(tabstop3,menuLine,text_str_11k);

	if ((sound_rate==22050)&&((menuMisc!=1)||(bb)))
		write_text_inv(tabstop5,menuLine,text_str_22k);
	else
		write_text(tabstop5,menuLine,text_str_22k);

	if ((sound_rate==32000)&&((menuMisc!=1)||(bb)))
		write_text_inv(tabstop7,menuLine,text_str_32k);
	else
		write_text(tabstop7,menuLine,text_str_32k);

	if ((sound_rate==44100)&&((menuMisc!=1)||(bb)))
		write_text_inv(tabstop9,menuLine,text_str_44k);
	else
		write_text(tabstop9,menuLine,text_str_44k);

	// 3
	menuLine+=2;
	write_text(leftMargin,menuLine,"Chip Memory Size");
	if ((!mainMenu_chipMemory)&&((menuMisc!=2)||(bb)))
		write_text_inv(tabstop3,menuLine,"512kB");
	else
		write_text(tabstop3,menuLine,"512kB");

	if ((mainMenu_chipMemory==1)&&((menuMisc!=2)||(bb)))
		write_text_inv(tabstop6+1,menuLine,"1MB");
	else
		write_text(tabstop6+1,menuLine,"1MB");

	if ((mainMenu_chipMemory==2)&&((menuMisc!=2)||(bb)))
		write_text_inv(tabstop9+1,menuLine,"2MB");
	else
		write_text(tabstop9+1,menuLine,"2MB");

	// 4
	menuLine+=2;
	write_text(leftMargin,menuLine,"Bogo Memory Size");
	if ((!mainMenu_bogoMemory)&&((menuMisc!=3)||(bb)))
		write_text_inv(tabstop3,menuLine,"none");
	else
		write_text(tabstop3,menuLine,"none");

	if ((mainMenu_bogoMemory==1)&&((menuMisc!=3)||(bb)))
		write_text_inv(tabstop6,menuLine,"512kB");
	else
		write_text(tabstop6,menuLine,"512kB");

	if ((mainMenu_bogoMemory==2)&&((menuMisc!=3)||(bb)))
		write_text_inv(tabstop9+1,menuLine,"1MB");
	else
		write_text(tabstop9+1,menuLine,"1MB");

	// 5
	menuLine+=2;
	write_text(leftMargin,menuLine,"Kickstart");
	if ((kickstart==0)&&((menuMisc!=4)||(bb)))
		write_text_inv(tabstop3,menuLine,"1.2");
	else
		write_text(tabstop3,menuLine,"1.2");

	if ((kickstart==1)&&((menuMisc!=4)||(bb)))
		write_text_inv(tabstop5,menuLine,"1.3");
	else
		write_text(tabstop5,menuLine,"1.3");

	if ((kickstart==2)&&((menuMisc!=4)||(bb)))
		write_text_inv(tabstop7,menuLine,"2.0");
	else
		write_text(tabstop7,menuLine,"2.0");

	// 6
	menuLine+=2;
	write_text(leftMargin,menuLine,"Pandora CPU-Speed");
	sprintf((char*)cpuSpeed, "%d",mainMenu_cpuSpeed);
	if ((menuMisc!=5)||(bb))
		write_text_inv(tabstop4-1,menuLine,cpuSpeed);
	else
		write_text(tabstop4-1,menuLine,cpuSpeed);
	write_text(tabstop6-1,menuLine,"MHz");

	menuLine++;
	write_text(leftMargin,menuLine,text_str_misc_separator);
	menuLine++;

	// 7
	write_text(leftMargin,menuLine,"Control config");

	if ((mainMenu_joyConf==1)&&((menuMisc!=6)||(bb)))
		write_text_inv(tabstop6,menuLine,"1");
	else
		write_text(tabstop6,menuLine,"1");

	if ((mainMenu_joyConf==2)&&((menuMisc!=6)||(bb)))
		write_text_inv(tabstop7,menuLine,"2");
	else
		write_text(tabstop7,menuLine,"2");

	menuLine+=2;
	if (mainMenu_joyConf==0) write_text(5,menuLine,"Standard buttons");
	else if (mainMenu_joyConf==1) write_text(5,menuLine,"Jump=X - Button 1=B");
	else if (mainMenu_joyConf==2) write_text(5,menuLine,"Jump=B - Button 1=X");

	// 8
	menuLine+=2;
	write_text(leftMargin, menuLine,text_str_status_line);
	if ((!mainMenu_showStatus)&&((menuMisc!=7)||(bb)))
		write_text_inv(tabstop1,menuLine, "Off");
	else
		write_text(tabstop1, menuLine, "Off");
	if ((mainMenu_showStatus)&&((menuMisc!=7)||(bb)))
		write_text_inv(tabstop3, menuLine,"On");
	else
		write_text(tabstop3, menuLine,"On");

	menuLine++;
	write_text(leftMargin,menuLine,text_str_misc_separator);
	menuLine++;

	// 9
	write_text(leftMargin,menuLine,text_str_mouse_multiplier);

	if ((mainMenu_mouseMultiplier==25)&&((menuMisc!=8)||(bb)))
		write_text_inv(tabstop1,menuLine,text_str_025x);
	else
		write_text(tabstop1,menuLine,text_str_025x);

	if ((mainMenu_mouseMultiplier==50)&&((menuMisc!=8)||(bb)))
		write_text_inv(tabstop3,menuLine,text_str_05x);
	else
		write_text(tabstop3,menuLine,text_str_05x);

	if ((mainMenu_mouseMultiplier==1)&&((menuMisc!=8)||(bb)))
		write_text_inv(tabstop5,menuLine,text_str_1x);
	else
		write_text(tabstop5,menuLine,text_str_1x);

	if ((mainMenu_mouseMultiplier==2)&&((menuMisc!=8)||(bb)))
		write_text_inv(tabstop7,menuLine,text_str_2x);
	else
		write_text(tabstop7,menuLine,text_str_2x);

	if ((mainMenu_mouseMultiplier==4)&&((menuMisc!=8)||(bb)))
		write_text_inv(tabstop9,menuLine,text_str_4x);
	else
		write_text(tabstop9,menuLine,text_str_4x);

	// 10
	menuLine+=2;
	write_text(leftMargin,menuLine,text_str_stylus_offset);

	if ((mainMenu_stylusOffset==0)&&((menuMisc!=9)||(bb)))
		write_text_inv(tabstop1,menuLine,text_str_0px);
	else
		write_text(tabstop1,menuLine,text_str_0px);

	if ((mainMenu_stylusOffset==2)&&((menuMisc!=9)||(bb)))
		write_text_inv(tabstop3,menuLine,text_str_1px);
	else
		write_text(tabstop3,menuLine,text_str_1px);

	if ((mainMenu_stylusOffset==6)&&((menuMisc!=9)||(bb)))
		write_text_inv(tabstop5,menuLine,text_str_3px);
	else
		write_text(tabstop5,menuLine,text_str_3px);

	if ((mainMenu_stylusOffset==10)&&((menuMisc!=9)||(bb)))
		write_text_inv(tabstop7,menuLine,text_str_5px);
	else
		write_text(tabstop7,menuLine,text_str_5px);

	if ((mainMenu_stylusOffset==16)&&((menuMisc!=9)||(bb)))
		write_text_inv(tabstop9,menuLine,text_str_8px);
	else
		write_text(tabstop9,menuLine,text_str_8px);

	// 11
	menuLine+=2;
	write_text(leftMargin,menuLine,text_str_tap_delay);

	if ((mainMenu_tapDelay==10)&&((menuMisc!=10)||(bb)))
		write_text_inv(tabstop1,menuLine,text_str_normal);
	else
		write_text(tabstop1,menuLine,text_str_normal);

	if ((mainMenu_tapDelay==5)&&((menuMisc!=10)||(bb)))
		write_text_inv(tabstop5,menuLine,text_str_short);
	else
		write_text(tabstop5,menuLine,text_str_short);

	if ((mainMenu_tapDelay==2)&&((menuMisc!=10)||(bb)))
		write_text_inv(tabstop9,menuLine,text_str_none);
	else
		write_text(tabstop9,menuLine,text_str_none);

	menuLine++;
	write_text(leftMargin,menuLine,text_str_misc_separator);
	menuLine++;

	/*// 14
	write_text(leftMargin,menuLine,"Scanlines (for 2x pixels mode only)");
	menuLine+=2;
	if ((mainMenu_scanlines==0)&&((menuMisc!=13)||(bb)))
		write_text_inv(13,menuLine,"no");
	else
		write_text(13,menuLine,"no");

	if ((mainMenu_scanlines==1)&&((menuMisc!=13)||(bb)))
		write_text_inv(tabstop1,menuLine,"H1");
	else
		write_text(tabstop1,menuLine,"H1");

	if ((mainMenu_scanlines==2)&&((menuMisc!=13)||(bb)))
		write_text_inv(tabstop3,menuLine,"H2");
	else
		write_text(tabstop3,menuLine,"H2");

	if ((mainMenu_scanlines==3)&&((menuMisc!=13)||(bb)))
		write_text_inv(tabstop5,menuLine,"V1");
	else
		write_text(tabstop5,menuLine,"V1");

	if ((mainMenu_scanlines==4)&&((menuMisc!=13)||(bb)))
		write_text_inv(tabstop7,menuLine,"V2");
	else
		write_text(tabstop7,menuLine,"V2");

	if ((mainMenu_scanlines==5)&&((menuMisc!=13)||(bb)))
		write_text_inv(tabstop9,menuLine,"grid");
	else
		write_text(tabstop9,menuLine,"grid");*/


	text_flip();
	b++;
}

static int key_miscMenu(int *c)
{
	int end=0;
	static int delay=0;
	int left=0, right=0, up=0, down=0, hit0=0, hit1=0;
	delay ++;
	if (delay<5) return end;
	delay=0;

#if !defined(GCW0)

	SDL_Joystick *joy = uae4all_joy0 ;
	SDL_JoystickUpdate ();
#ifdef PANDORA
	if (     SDL_JoystickGetAxis(joy, 0) > 0) right=1;
	else if (SDL_JoystickGetAxis(joy, 0) < 0) left=1;
	else if (SDL_JoystickGetAxis(joy, 1) < 0) up=1;
	else if (SDL_JoystickGetAxis(joy, 1) > 0) down=1;
#else
	if (SDL_JoystickGetButton(joy, GP2X_BUTTON_RIGHT)) right=1;
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_LEFT)) left=1;
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_UP)) up=1;
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_DOWN)) down=1;
#endif
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_B)) hit0=1;
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_A)) hit0=1;
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_START)) {printf("Pressed Start\n");hit1=1;}
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_X)) hit0=1;

#else
	SDL_Event event;

	while (SDL_PollEvent(&event) > 0)
	{
		if (event.type == SDL_JOYBUTTONDOWN)
		{
			switch (event.jbutton.button)
			{

			case GP2X_BUTTON_RIGHT: right=1; break;
			case GP2X_BUTTON_LEFT: left=1; break;
			case GP2X_BUTTON_UP: up=1; break;
			case GP2X_BUTTON_DOWN: down=1; break;
			case GP2X_BUTTON_B: hit0=1; break;
			case GP2X_BUTTON_START: hit1=1; break;
			case GP2X_BUTTON_X: hit0=1; break;
			case GP2X_BUTTON_A: hit0=1; break;
			}
		}
		else{
			if (event.type == SDL_KEYDOWN)
			{
				uae4all_play_click();
				switch(event.key.keysym.sym)
				{
				case SDLK_RIGHT: right=1; break;
				case SDLK_LEFT: left=1; break;
				case SDLK_UP: up=1; break;
				case SDLK_DOWN: down=1; break;
				case SDLK_RETURN:
				case SDLK_LCTRL: hit0=1; break;
				case SDLK_LALT: hit1=1; break;
				}
			}
		}
	}

#endif
		if (hit0)
		{
			end = -1;
		}
		else if (hit1)
		{
			end = -1;
		}
		else if (up)
		{
			if (menuMisc==0) menuMisc=10;
			else menuMisc--;
		}
		else if (down)
		{
			if (menuMisc==10) menuMisc=0;
			else menuMisc++;
		}
		switch (menuMisc)
		{
			case 1:
				if ((left)||(right))
				{
					static int rates[] = { 8000, 11025, 22050, 32000, 44100 };
					int sel;
					for (sel = 0; sel < sizeof(rates) / sizeof(rates[0]); sel++)
						if (rates[sel] == sound_rate) break;
					sel += left ? -1 : 1;
					if (sel < 0) sel = 4;
					if (sel > 4) sel = 0;
					sound_rate = rates[sel];
				}
				break;
			case 2:
				if (left)
				{
					if (mainMenu_chipMemory>0)
						mainMenu_chipMemory--;
					else
						mainMenu_chipMemory=2;
				}
				else if (right)
				{
					if (mainMenu_chipMemory<2)
						mainMenu_chipMemory++;
					else
						mainMenu_chipMemory=0;
				}
				break;
			case 3:
				if (left)
				{
					if (mainMenu_bogoMemory>0)
						mainMenu_bogoMemory--;
					else
						mainMenu_bogoMemory=2;
				}
				else if (right)
				{
					if (mainMenu_bogoMemory<2)
						mainMenu_bogoMemory++;
					else
						mainMenu_bogoMemory=0;
				}
				break;
			case 4:
				if (left)
				{
					if (kickstart>0)
						kickstart--;
					else
						kickstart=2;
				}
				else if (right)
				{
					if (kickstart<2)
						kickstart++;
					else
						kickstart=0;
				}
				break;
			case 5:
				if(left)
					mainMenu_cpuSpeed-=10;
				else if(right)
					mainMenu_cpuSpeed+=10;
				break;
			case 6:
				if (left || right)
				{
					if (mainMenu_joyConf>1)
						mainMenu_joyConf=1;
					else
						mainMenu_joyConf=2;
				}
 				break;
			case 7:
				if ((left)||(right))
					mainMenu_showStatus=!mainMenu_showStatus;
				break;
			case 8:
				if (left)
				{
					if (mainMenu_mouseMultiplier == 4)
						mainMenu_mouseMultiplier = 2;
					else if (mainMenu_mouseMultiplier == 2)
						mainMenu_mouseMultiplier = 1;
					else if (mainMenu_mouseMultiplier == 1)
						mainMenu_mouseMultiplier = 50;
					else if (mainMenu_mouseMultiplier == 50)
						mainMenu_mouseMultiplier = 25;
					else
						mainMenu_mouseMultiplier = 4;
				}
				else if (right)
				{
					if (mainMenu_mouseMultiplier == 4)
						mainMenu_mouseMultiplier = 25;
					else if (mainMenu_mouseMultiplier == 2)
						mainMenu_mouseMultiplier = 4;
					else if (mainMenu_mouseMultiplier == 1)
						mainMenu_mouseMultiplier = 2;
					else if (mainMenu_mouseMultiplier == 50)
						mainMenu_mouseMultiplier = 1;
					else
						mainMenu_mouseMultiplier = 50;
				}
				break;
			case 9:
				if (left)
				{
					if (mainMenu_stylusOffset == 0)
						mainMenu_stylusOffset = 16;
					else if (mainMenu_stylusOffset == 2)
						mainMenu_stylusOffset = 0;
					else if (mainMenu_stylusOffset == 6)
						mainMenu_stylusOffset = 2;
					else if (mainMenu_stylusOffset == 10)
						mainMenu_stylusOffset = 6;
					else
						mainMenu_stylusOffset = 10;
				}
				else if (right)
				{
					if (mainMenu_stylusOffset == 0)
						mainMenu_stylusOffset = 2;
					else if (mainMenu_stylusOffset == 2)
						mainMenu_stylusOffset = 6;
					else if (mainMenu_stylusOffset == 6)
						mainMenu_stylusOffset = 10;
					else if (mainMenu_stylusOffset == 10)
						mainMenu_stylusOffset = 16;
					else
						mainMenu_stylusOffset = 0;
				}
				break;
			case 10:
				if (left)
				{
					if (mainMenu_tapDelay == 10)
						mainMenu_tapDelay = 2;
					else if (mainMenu_tapDelay == 5)
						mainMenu_tapDelay = 10;
					else
						mainMenu_tapDelay = 5;
				}
				else if (right)
				{
					if (mainMenu_tapDelay == 10)
						mainMenu_tapDelay = 5;
					else if (mainMenu_tapDelay == 5)
						mainMenu_tapDelay = 2;
					else
						mainMenu_tapDelay = 10;
				}
				break;
			/*case 13:
				if(left)
				{
					if(mainMenu_scanlines == 0)
						mainMenu_scanlines = 5;
					else
						mainMenu_scanlines--;
				}
				else if(right)
				{
					if(mainMenu_scanlines == 5)
						mainMenu_scanlines = 0;
					else
						mainMenu_scanlines++;
				}*/
		}
	//}

	return end;
}

static void raise_miscMenu()
{
	int i;

	text_draw_background();
	text_flip();
	for(i=0;i<10;i++)
	{
		text_draw_background();
		text_draw_window(80-64,(10-i)*24,160+64+64,220,text_str_misc_title);
		text_flip();
	}
}

static void unraise_miscMenu()
{
	int i;

	for(i=9;i>=0;i--)
	{
		text_draw_background();
		text_draw_window(80-64,(10-i)*24,160+64+64,220,text_str_misc_title);
		text_flip();
	}
	text_draw_background();
	text_flip();
}
int run_menuMisc()
{
	SDL_Event event;
	SDL_Delay(150);
	while(SDL_PollEvent(&event))
		SDL_Delay(10);
	int end=0,c=0;
	raise_miscMenu();
	while(!end)
	{
		draw_miscMenu(c);
		end=key_miscMenu(&c);
	}
	set_joyConf();
	unraise_miscMenu();
	return end;
}
