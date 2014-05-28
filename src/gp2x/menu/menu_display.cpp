#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "sysconfig.h"
#include "sysdeps.h"
#include "uae.h"
#include "options.h"
#include "sound.h"
#include <SDL.h>
#include "gp2x.h"
#include <SDL_ttf.h>

char *text_str_display_separator="----------------------------------";
char *text_str_display_title=    "        Display Settings        -";

#define MAX_CUSTOM_ID 96
#define MIN_CUSTOM_ID -5

int menuDisplay = 0;
extern int moveY;
extern int mainMenu_displayedLines;
extern int screenWidth;
extern int mainMenu_cutLeft;
extern int mainMenu_cutRight;

extern int mainMenu_ntsc;
extern int mainMenu_frameskip;

extern char presetMode[20];
extern int presetModeId;

char value[20]="";

extern SDL_Joystick *uae4all_joy0;

extern SDL_Surface *prSDLScreen;

static void getMapping(int customId)
{
}

static void draw_displayMenu(int c)
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
	text_draw_window(2,2,40,30,text_str_display_title);

	// 1
	if (menuDisplay == 0 && bb)
		write_text_inv(3, menuLine, "Return to main menu");
	else
		write_text(3, menuLine, "Return to main menu");

	menuLine++;
	write_text(leftMargin,menuLine,text_str_display_separator);
	menuLine++;

	// 2
	write_text(leftMargin,menuLine,"Preset Mode");
	if ((menuDisplay!=1)||(bb))
		write_text(tabstop3,menuLine,presetMode);
	else
		write_text_inv(tabstop3,menuLine,presetMode);

	menuLine++;
	write_text(leftMargin,menuLine,text_str_display_separator);
	menuLine+=2;
	write_text(leftMargin,menuLine,"Custom Settings");
	menuLine++;
	write_text(leftMargin,menuLine,"---------------");
	menuLine++;

	// 3
	write_text(leftMargin,menuLine,"Displayed Lines");
	sprintf(value, "%d", mainMenu_displayedLines);
	if ((menuDisplay!=2)||(bb))
		write_text(tabstop3,menuLine,value);
	else
		write_text_inv(tabstop3,menuLine,value);

	// 4
	menuLine+=2;
	write_text(leftMargin,menuLine,"Screen Width");
	sprintf(value, "%d", screenWidth);
	if ((menuDisplay!=3)||(bb))
		write_text(tabstop3,menuLine,value);
	else
		write_text_inv(tabstop3,menuLine,value);

	// 5
	menuLine+=2;
	write_text(leftMargin,menuLine,"Vertical Position");
	sprintf(value, "%d", moveY);
	if ((menuDisplay!=4)||(bb))
		write_text(tabstop3,menuLine,value);
	else
		write_text_inv(tabstop3,menuLine,value);

	// 6
	menuLine+=2;
	write_text(leftMargin,menuLine,"Cut Left");
	sprintf(value, "%d", mainMenu_cutLeft);
	if ((menuDisplay!=5)||(bb))
		write_text(tabstop3,menuLine,value);
	else
		write_text_inv(tabstop3,menuLine,value);

	// 7
	menuLine+=2;
	write_text(leftMargin,menuLine,"Cut Right");
	sprintf(value, "%d", mainMenu_cutRight);
	if ((menuDisplay!=6)||(bb))
		write_text(tabstop3,menuLine,value);
	else
		write_text_inv(tabstop3,menuLine,value);

	// 8
	menuLine++;
	write_text(leftMargin,menuLine,text_str_display_separator);
	menuLine++;
	write_text(leftMargin,menuLine,"Frameskip");
	if ((mainMenu_frameskip==0)&&((menuDisplay!=7)||(bb)))
		write_text_inv(tabstop1,menuLine,"0");
	else
		write_text(tabstop1,menuLine,"0");
	if ((mainMenu_frameskip==1)&&((menuDisplay!=7)||(bb)))
		write_text_inv(tabstop3,menuLine,"1");
	else
		write_text(tabstop3,menuLine,"1");

	// 9
	menuLine+=2;
	write_text(leftMargin,menuLine,"Refresh Rate");
	if ((!mainMenu_ntsc)&&((menuDisplay!=8)||(bb)))
		write_text_inv(tabstop1,menuLine,"50Hz");
	else
		write_text(tabstop1,menuLine,"50Hz");

	if ((mainMenu_ntsc)&&((menuDisplay!=8)||(bb)))
		write_text_inv(tabstop3+1,menuLine,"60Hz");
	else
		write_text(tabstop3+1,menuLine,"60Hz");

	text_flip();
	b++;
}

static int key_displayMenu(int *c)
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
		printf("event.type %d\n", event.type);
		printf("event.jbutton.button %d\n", event.jbutton.button);
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
			if (menuDisplay==0) menuDisplay=8;
			else menuDisplay--;
		}
		else if (down)
		{
			if (menuDisplay==8) menuDisplay=0;
			else menuDisplay++;
		}
		switch (menuDisplay)
		{
			case 1:
				if (left)
				{
					switch(presetModeId)
					{
						case 0:
							presetModeId=7;
							strcpy(presetMode, "320x200 fullscreen");
							mainMenu_displayedLines=200;
							screenWidth = 800;
							break;
						case 1:
							presetModeId=0;
							strcpy(presetMode, "320x200 upscaled");
							mainMenu_displayedLines=200;
							screenWidth = 768;
							break;
						case 2:
							presetModeId=1;
							strcpy(presetMode, "320x216 upscaled");
							mainMenu_displayedLines=216;
							screenWidth = 716;
							break;
						case 3:
							presetModeId=2;
							strcpy(presetMode, "320x240 upscaled");
							mainMenu_displayedLines=240;
							screenWidth = 640;
							break;
						case 4:
							presetModeId=3;
							strcpy(presetMode, "320x256 upscaled");
							mainMenu_displayedLines=256;
							screenWidth = 600;
							break;
						case 5:
							presetModeId=4;
							strcpy(presetMode, "320x262 upscaled");
							mainMenu_displayedLines=262;
							screenWidth = 588;
							break;
						case 6:
							presetModeId=5;
							strcpy(presetMode, "320x270 upscaled");
							mainMenu_displayedLines=270;
							screenWidth = 570;
							break;
						case 7:
							presetModeId=6;
							strcpy(presetMode, "320x200 NTSC");
							mainMenu_displayedLines=200;
							screenWidth = 640;
							break;
					}
					update_display();
				}
				else if (right)
				{
					switch(presetModeId)
					{
						case 0:
							presetModeId=1;
							strcpy(presetMode, "320x216 upscaled");
							mainMenu_displayedLines=216;
							screenWidth = 716;
							break;
						case 1:
							presetModeId=2;
							strcpy(presetMode, "320x240 upscaled");
							mainMenu_displayedLines=240;
							screenWidth = 640;
							break;
						case 2:
							presetModeId=3;
							strcpy(presetMode, "320x256 upscaled");
							mainMenu_displayedLines=256;
							screenWidth = 600;
							break;
						case 3:
							presetModeId=4;
							strcpy(presetMode, "320x262 upscaled");
							mainMenu_displayedLines=262;
							screenWidth = 588;
							break;
						case 4:
							presetModeId=5;
							strcpy(presetMode, "320x270 upscaled");
							mainMenu_displayedLines=270;
							screenWidth = 570;
							break;
						case 5:
							presetModeId=6;
							strcpy(presetMode, "320x200 NTSC");
							mainMenu_displayedLines=200;
							screenWidth = 640;
							break;
						case 6:
							presetModeId=7;
							strcpy(presetMode, "320x200 fullscreen");
							mainMenu_displayedLines=200;
							screenWidth = 800;
							break;
						case 7:
							presetModeId=0;
							strcpy(presetMode, "320x200 upscaled");
							mainMenu_displayedLines=200;
							screenWidth = 768;
							break;
					}
					update_display();
				}
				break;
			case 2:
				if (left)
				{
					if (mainMenu_displayedLines>100)
						mainMenu_displayedLines--;
					update_display();
				}
				else if (right)
				{
					if (mainMenu_displayedLines<270)
						mainMenu_displayedLines++;
					update_display();
				}
				break;
			case 3:
				if (left)
				{
					screenWidth-=10;
					if (screenWidth<200)
						screenWidth=200;
					update_display();
				}
				else if (right)
				{
					screenWidth+=10;
					if (screenWidth>800)
						screenWidth=800;
					update_display();
				}
				break;
			case 4:
				if (left)
				{
					if (moveY>-42)
						moveY--;
				}
				else if (right)
				{
					if (moveY<50)
						moveY++;
				}
				break;
			case 5:
				if (left)
				{
					if (mainMenu_cutLeft>0)
						mainMenu_cutLeft--;
					update_display();
				}
				else if (right)
				{
					if (mainMenu_cutLeft<100)
						mainMenu_cutLeft++;
					update_display();
				}
				break;
			case 6:
				if (left)
				{
					if (mainMenu_cutRight>0)
						mainMenu_cutRight--;
					update_display();
				}
				else if (right)
				{
					if (mainMenu_cutRight<100)
						mainMenu_cutRight++;
					update_display();
				}
				break;
			case 7:
				if ((left)||(right))
						mainMenu_frameskip = !mainMenu_frameskip;
				break;
			case 8:
				if ((left)||(right))
						mainMenu_ntsc = !mainMenu_ntsc;
				break;
		}
	//}

	return end;
}

static void raise_displayMenu()
{
	update_display();
	int i;
	text_draw_background();
	text_flip();
	for(i=0;i<10;i++)
	{
		text_draw_background();
		text_draw_window(80-64,(10-i)*24,160+64+64,220,text_str_display_title);
		text_flip();
	}
}

static void unraise_displayMenu()
{
	int i;

	for(i=9;i>=0;i--)
	{
		text_draw_background();
		text_draw_window(80-64,(10-i)*24,160+64+64,220,text_str_display_title);
		text_flip();
	}
	text_draw_background();
	text_flip();
}

int run_menuDisplay()
{
	SDL_Event event;
	SDL_Delay(150);
	while(SDL_PollEvent(&event))
		SDL_Delay(10);
	int end=0, c=0;
	raise_displayMenu();
	while(!end)
	{
		draw_displayMenu(c);
		end=key_displayMenu(&c);
	}
	set_joyConf();
	unraise_displayMenu();
	return end;
}
