#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <SDL.h>

#include "menu.h"
#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "sound.h"
#include "zfile.h"
#include "gp2x.h"
#include "disk.h"
static int norelaunchGP2XMenu = 0;
#include "menu_load.h"
//#include "gp2xutil.h"
#include "cpuspeed/cpuctrl.h"
#include "savestate.h"
extern int text_dir_num_files_index;
int init_sound(void);
void gp2x_stop_sound(void);
extern char launchDir[300];
extern char currentDir[300];

extern int emulating;
extern void setBatteryLED(int);
extern int flashLED;
extern int gp2xClockSpeed;
extern int sound_rate;
#ifdef CYCLONE
extern int timeslice_mode;
#else
int timeslice_mode=2;
#endif
extern int skipintro;
extern char launchDir[300];
extern int bReloadKickstart;
char *statusmessages[] = { "AUTOFIRE ON\0", "AUTOFIRE OFF\0","SCREENSHOT SAVED\0","SCRIPT SAVED\0","SCRIPT AND SCREENSHOT SAVED\0"};
int showmsg=0;

extern char uae4all_image_file0[128];
extern char uae4all_image_file1[128];
extern char uae4all_image_file2[128];
extern char uae4all_image_file3[128];
char filename0[128];
char filename1[128];
char filename2[128];
char filename3[128];
static char *text_str_title=    "----- UAE4All Pandora -----";
static char *text_str_load=     "Select Image Disk (L)";
static char *text_str_df0=		"DF0:";
static char *text_str_df1=		"DF1:";
static char *text_str_df2=		"DF2:";
static char *text_str_df3=		"DF3:";
static char *text_str_number_of_drives=		"Number of drives";
static char *text_str_flash_led="Drive LED";
static char *text_str_throttle= "System clock";
static char *text_str_sync_trsh="Sync threshold";
static char *text_str_display="Display Settings (L)";
static char *text_str_savestates="Savestates";
static char *text_str_ham="HAM emulation";
static char *text_str_frameskip="Frameskip";
static char *text_str_videomode="Video mode";
static char *text_str_0="0";
static char *text_str_1="1";
static char *text_str_2="2";
static char *text_str_3="3";
static char *text_str_4="4";
static char *text_str_5="5";
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
static char *text_str_44k="44k";
static char *text_str_32k="32k";
static char *text_str_22k="22k";
static char *text_str_11k="11k";
static char *text_str_8k="8k";
static char *text_str_100p="100";
static char *text_str_83p="83";
static char *text_str_75p="75";
static char *text_str_50p="50";
static char *text_str_25p="25";
static char *text_str_pal="PAL";
static char *text_str_ntsc="NTSC";
static char *text_str_auto="auto";
static char *text_str_eject="Eject All Drives";
//static char *text_str_gp2x_clock="Clock MHz";
static char *text_str_gp2x_clock_none="none";
static char *text_str_gp2x_clock_200="200";
static char *text_str_gp2x_clock_250="250";
static char *text_str_gp2x_clock_275="275";
static char *text_str_sound="Sound";
static char *text_str_on="on";
static char *text_str_fast="fast";
static char *text_str_accurate="accurate";
static char *text_str_off="off";
static char *text_str_faked="faked";
static char *text_str_separator="--------------------------------";
static char *text_str_reset="Reset (R)";
static char *text_str_run=  "Run (Start)";
static char *text_str_exit= "Exit";
static char *text_str_autosave="Autosave";
static char *text_str_sndrate="Sound rate";

static bool  showwarning=false;

int mainMenu_throttle=0;
#if !defined(DEBUG_UAE4ALL) && !defined(PROFILER_UAE4ALL) && !defined(AUTO_RUN) && !defined(AUTO_FRAMERATE)
int mainMenu_frameskip=0;
#else
#ifdef PROFILER_UAE4ALL
#ifndef AUTO_PROFILER
int mainMenu_frameskip=0;
#else
int mainMenu_frameskip=0;
#endif
#else
#ifdef DEBUG_FRAMERATE
int mainMenu_frameskip=0;
#else
int mainMenu_frameskip=0;
#endif
#endif
#endif
int def_frameskip=mainMenu_frameskip;
#define DEFAULT_FRAMESKIP def_frameskip

int mainMenu_showStatus=DEFAULT_STATUSLN;
int mainMenu_stylusOffset=0;
int mainMenu_tapDelay=10;
int mainMenu_mouseMultiplier = DEFAULT_MOUSEMULTIPLIER;
int mainMenu_sound=DEFAULT_SOUND;
int mainMenu_case=-1;
int mainMenu_autosave=DEFAULT_AUTOSAVE;
int mainMenu_ntsc=DEFAULT_NTSC;
//int mainMenu_useOneMegaChip=DEFAULT_USE1MBCHIP;
int mainMenu_chipMemory=1;
int mainMenu_bogoMemory=0;
int mainMenu_joyConf=DEFAULT_JOYCONF;
int mainMenu_scanlines=0;
int mainMenu_button1=GP2X_BUTTON_X;
int mainMenu_button2=GP2X_BUTTON_B;
int mainMenu_jump=GP2X_BUTTON_A;
int mainMenu_autofire=DEFAULT_AUTOFIRE;
int mainMenu_drives=DEFAULT_DRIVES;
int mainMenu_floppyspeed=100;
int mainMenu_ham=1;
extern int enableHWscaling;
extern int gp2xMouseEmuOn;
extern int gp2xButtonRemappingOn;
extern SDL_Joystick *uae4all_joy0;
int mainMenu_enableScreenshots=DEFAULT_ENABLESCREENSHOTS;
int mainMenu_enableScripts=DEFAULT_ENABLESCRIPTS;
int nr_drives=DEFAULT_DRIVES;
int current_drive=0;
extern int kickstart;
extern int oldkickstart;
char presetMode[20]="320x240 upscaled";
int presetModeId=2;
extern int moveX;
extern int moveY;
int mainMenu_displayedLines = 240;
extern int screenWidth;
int mainMenu_cutLeft = 0;
int mainMenu_cutRight = 0;

int mainMenu_customControls = 0;
int mainMenu_custom_dpad = 0;
int mainMenu_custom_up = 0;
int mainMenu_custom_down = 0;
int mainMenu_custom_left = 0;
int mainMenu_custom_right = 0;
int mainMenu_custom_A = 0;
int mainMenu_custom_B = 0;
int mainMenu_custom_X = 0;
int mainMenu_custom_Y = 0;
int mainMenu_custom_L = 0;
int mainMenu_custom_R = 0;

int mainMenu_cpuSpeed=600;
int lastCpuSpeed=600;
char speedCmd[50];
int ntsc=0;

extern SDL_Surface *prSDLScreen;

int createScript(int bIcon)
{
	if(!uae4all_image_file0[0]) return 0;
	char path[512];
	char script[512];
	char buffer[128];
	char *p;
	if (!strlen(uae4all_image_file0)) return 0;
	p=uae4all_image_file0+strlen(uae4all_image_file0)-1;
	while (*p != '/')p--;
	p++;
	int len=strlen(p);
	char gamename[len];
	strcpy(gamename,p);
	char * pch;
	pch = strstr (gamename,"(");
	if (pch) strcpy(pch,"\0");
	else{
		pch = strstr (gamename,".adf");
		if (pch) strcpy(pch,"\0");
	}
	if (uae4all_image_file0[0])	sprintf(path, "%s/scripts/%s.gpe", launchDir,gamename);
	else return 0;
	FILE *f=fopen(path,"w");
	if (!f) return 0;
	strcpy(script,"#!/bin/sh\n");
	strcat(script,"./cpu_speed --upll --timing 1 --cpuclk 266 --trc 6 --tras 4 --twr 1 --tmrd 1 --trfc 1 --trp 2 --trcd 2\n");
	strcat(script,"sleep 1\n");

	strcat(script,"cd ..\n");
	strcat(script,"./uae4all.gpe ");
	sprintf(buffer,"-df0 \"%s\" ",uae4all_image_file0);
	strcat(script,buffer);

	if(uae4all_image_file1[0]){
		sprintf(buffer,"-df1 \"%s\" ",uae4all_image_file1);
		strcat(script,buffer);
	}

	if(uae4all_image_file2[0]){
		sprintf(buffer,"-df2 \"%s\" ",uae4all_image_file2);
		strcat(script,buffer);
	}

	if(uae4all_image_file3[0]){
		sprintf(buffer,"-df3 \"%s\" ",uae4all_image_file3);
		strcat(script,buffer);
	}


	if (mainMenu_mouseMultiplier!=DEFAULT_MOUSEMULTIPLIER) {
		sprintf(buffer,"-mousemultiplier %d ",mainMenu_mouseMultiplier);
		strcat(script,buffer);
	}


	if (mainMenu_sound!=DEFAULT_SOUND) {
		sprintf(buffer,"-sound %d ",mainMenu_sound);
		strcat(script,buffer);
	}

	if (sound_rate!=DEFAULT_SOUND_FREQ) {
		sprintf(buffer,"-soundrate %d ",sound_rate);
		strcat(script,buffer);
	}
	if (mainMenu_throttle!=DEFAULT_SYSTEMCLOCK) {
		sprintf(buffer,"-systemclock %d ",mainMenu_throttle);
		strcat(script,buffer);
	}

	if (timeslice_mode!=DEFAULT_SYNCTHRESHOLD) {
		sprintf(buffer,"-syncthreshold %d ",timeslice_mode);
		strcat(script,buffer);
	}

	if (mainMenu_autosave!=DEFAULT_AUTOSAVE){
		sprintf(path,"-autosave %d ",mainMenu_autosave);
		strcat(script,buffer);
	}

	if (mainMenu_frameskip!=DEFAULT_FRAMESKIP) {
		sprintf(buffer,"-frameskip %d ",mainMenu_frameskip);
		strcat(script,buffer);
	}

	if (mainMenu_joyConf!=DEFAULT_JOYCONF) {
		sprintf(buffer,"-joyconf %d ",mainMenu_joyConf);
		strcat(script,buffer);
	}

	if (mainMenu_ntsc!=DEFAULT_FRAMESKIP) {
		sprintf(buffer,"-ntsc %d ",mainMenu_ntsc);
		strcat(script,buffer);
	}

	if (mainMenu_drives!=DEFAULT_DRIVES) {
		sprintf(path,"-drives %d ",mainMenu_drives);
		strcat(script,buffer);
	}

	if (mainMenu_autofire!=DEFAULT_AUTOFIRE) {
		sprintf(buffer,"-autofire %d ",mainMenu_autofire);
		strcat(script,buffer);
	}

	//if (mainMenu_useOneMegaChip!=DEFAULT_USE1MBCHIP) {
	//	sprintf(buffer,"-use1mbchip %d ",mainMenu_useOneMegaChip);
	//	strcat(script,buffer);
	//}

	if (enableHWscaling!=DEFAULT_SCALING) {
		sprintf(buffer,"-scaling %d ",enableHWscaling);
		strcat(script,buffer);
	}

	if (kickstart!=DEFAULT_KICKSTART) {
		sprintf(buffer,"-kickstart %d ",kickstart);
		strcat(script,buffer);
	}


	strcat(script,"\n\0");

	fputs(script,f);
	fclose(f);
	sprintf(path, "%s/scripts/%s.png", launchDir,gamename);
	if (bIcon) save_thumb(ICON,path);
	return 1;

}

static void extractFileName(char * str,char *buffer)
{
	char *p=str+strlen(str)-1;
	while(*p != '/') p--;
	p++;
	strcpy(buffer,p);
}

static void adjustToWindow(char *str, char* buffer)
{
	if (strlen(str)<33) return;
	char *p=str+strlen(str)-13;
	for (int i=0;i<15;i++) {buffer[i]=*str;str++;}
	char tt[]={'.','.','.','\0',};
	//char *pp=&tt;
	strcat(buffer,tt);
	strcat(buffer,p);
}

static void showInfo()
{
	text_draw_background();
	char buffer[128];
	char buffertext[128];

	text_draw_window(2,2,35,20,"Info");
	SDL_Rect r;
	r.x=80-64; r.y=0; r.w=35*7; r.h=140;
	extern SDL_Surface *text_screen;
	SDL_SetClipRect(text_screen,&r);
	write_text(4,2,"Df0");

	extractFileName(uae4all_image_file0,buffer);
	adjustToWindow(buffer,buffertext);
	write_text(10,2,buffertext);

	write_text(4,4,"Df1");
	if (!uae4all_image_file1[0]) write_text(10,4,"Empty");
	else
	{
		extractFileName(uae4all_image_file1,buffer);
		adjustToWindow(buffer,buffertext);
		write_text(10,4,buffer);
	}

	write_text(4,6,"Df2");
	if (!uae4all_image_file2[0]) write_text(10,4,"Empty");
	else
	{
		extractFileName(uae4all_image_file2,buffer);
		adjustToWindow(buffer,buffertext);
		write_text(10,6,buffer);
	}

	write_text(4,8,"Df3");
	if (!uae4all_image_file3[0]) write_text(10,4,"Empty");
	else
	{
		extractFileName(uae4all_image_file3,buffer);
		adjustToWindow(buffer,buffertext);
		write_text(10,8,buffer);
	}

		text_flip();
		SDL_Event ev;
		SDL_Delay(333);
		while(SDL_PollEvent(&ev))
		SDL_Delay(10);
		while(!SDL_PollEvent(&ev))
				SDL_Delay(10);
		while(SDL_PollEvent(&ev))
				if (ev.type==SDL_QUIT)
					exit(1);
		SDL_Delay(200);
		SDL_SetClipRect(text_screen,NULL);
}

void set_joyConf()
{
	if (mainMenu_joyConf==0)
	{
		mainMenu_button1=GP2X_BUTTON_B;
		mainMenu_button2=GP2X_BUTTON_X;
		mainMenu_jump=0;
	}
	else if(mainMenu_joyConf==1)
	{
		mainMenu_button1=GP2X_BUTTON_B;
		mainMenu_button2=GP2X_BUTTON_A;
		mainMenu_jump=GP2X_BUTTON_X;
	}
	else if(mainMenu_joyConf==2)
	{
		mainMenu_button1=GP2X_BUTTON_X;
		mainMenu_button2=GP2X_BUTTON_A;
		mainMenu_jump=GP2X_BUTTON_B;
	}
}


static void draw_mainMenu(int c)
{
	/* New Menu
	0 = DF0:
	1 = DF1:
	2 = DF2:
	3 = DF3:
	4 = eject all drives
	5 = number of drives
	6 = floppy speed
	7 = display settings
	8 = sound
	9 = savestates (fame&uae core) / ham emulation (cyclone)
	10 = custom controls
	11 = more options
	12 = sound rate
	13 = reset
	14 = save config current game
	15 = save general config
	16 = exit
	*/
	/* Old Menu
	0 = load
	1 = sync threshold
	2 = frameskip
	3 = savestates (fame&uae core) / ham emulation (cyclone)
	4 = stylus offset
	5 = tap delay
	6 = mouse cursor speed
	7 = sound
	8 = eject external drives
	9 = sound rate
	10 = misc
	11 = reset
	12 = run
	13 = save config current game
	14 = save general config
	15 = exit
	*/
	static int b=0;
	int bb=(b%6)/3;
	int menuLine = 3;
	int leftMargin = 8;
	int tabstop1 = 17+4;
	int tabstop2 = 19+4;
	int tabstop3 = 21+4;
	int tabstop4 = 23+4;
	int tabstop5 = 25+4;
	int tabstop6 = 27+4;
	int tabstop7 = 29+4;
	int tabstop8 = 31+4;
	int tabstop9 = 33+4;

	text_draw_background();
	text_draw_window(leftMargin-1,menuLine-1,34,40,text_str_title);

	// 1
	if ((c==0)&&(bb))
		write_text_inv(leftMargin,menuLine,text_str_df0);
	else
		write_text(leftMargin,menuLine,text_str_df0);
	if(strcmp(uae4all_image_file0, "")==0)
		write_text_inv(13,menuLine,"insert disk image");
	else
		write_text_inv(13,menuLine,filename0);

	// 2
	menuLine+=2;
	if(nr_drives > 1)
	{
		if((c==1)&&(bb))
			write_text_inv(leftMargin,menuLine,text_str_df1);
		else
			write_text(leftMargin,menuLine,text_str_df1);
		if(strcmp(uae4all_image_file1, "")==0)
			write_text_inv(13,menuLine,"insert disk image");
		else
			write_text_inv(13,menuLine,filename1);
	}

	// 3
	menuLine+=2;
	if(nr_drives > 2)
	{
		if ((c==2)&&(bb))
			write_text_inv(leftMargin,menuLine,text_str_df2);
		else
			write_text(leftMargin,menuLine,text_str_df2);
		if(strcmp(uae4all_image_file2, "")==0)
			write_text_inv(13,menuLine,"insert disk image");
		else
			write_text_inv(13,menuLine,filename2);
	}

	// 4
	menuLine+=2;
	if(nr_drives > 3)
	{
		if ((c==3)&&(bb))
			write_text_inv(leftMargin,menuLine,text_str_df3);
		else
			write_text(leftMargin,menuLine,text_str_df3);
		if(strcmp(uae4all_image_file3, "")==0)
			write_text_inv(13,menuLine,"insert disk image");
		else
			write_text_inv(13,menuLine,filename3);
	}

	menuLine++;
	write_text(leftMargin,menuLine,text_str_separator);
	menuLine++;

	// 5
	if ((c==4)&&(bb))
		write_text_inv(leftMargin,menuLine,text_str_eject);
	else
		write_text(leftMargin, menuLine,text_str_eject);

	// 6
	menuLine+=2;
	write_text(leftMargin,menuLine,"Number of drives:");

	if ((nr_drives==1)&&((c!=5)||(bb)))
		write_text_inv(tabstop3,menuLine,"1");
	else
		write_text(tabstop3,menuLine,"1");

	if ((nr_drives==2)&&((c!=5)||(bb)))
		write_text_inv(tabstop4,menuLine,"2");
	else
		write_text(tabstop4,menuLine,"2");

	if ((nr_drives==3)&&((c!=5)||(bb)))
		write_text_inv(tabstop5,menuLine,"3");
	else
		write_text(tabstop5,menuLine,"3");

	if ((nr_drives==4)&&((c!=5)||(bb)))
		write_text_inv(tabstop6,menuLine,"4");
	else
		write_text(tabstop6,menuLine,"4");

	// 7
	menuLine+=2;
	write_text(leftMargin,menuLine,"Floppy speed:");

	if ((mainMenu_floppyspeed==100)&&((c!=6)||(bb)))
		write_text_inv(tabstop3,menuLine,"1x");
	else
		write_text(tabstop3,menuLine,"1x");

	if ((mainMenu_floppyspeed==200)&&((c!=6)||(bb)))
		write_text_inv(tabstop4+1,menuLine,"2x");
	else
		write_text(tabstop4+1,menuLine,"2x");

	if ((mainMenu_floppyspeed==400)&&((c!=6)||(bb)))
		write_text_inv(tabstop5+2,menuLine,"4x");
	else
		write_text(tabstop5+2,menuLine,"4x");

	if ((mainMenu_floppyspeed==800)&&((c!=6)||(bb)))
		write_text_inv(tabstop6+3,menuLine,"8x");
	else
		write_text(tabstop6+3,menuLine,"8x");

	menuLine++;
	write_text(leftMargin,menuLine,text_str_separator);
	menuLine++;

	// 8
	if ((c==7)&&(bb))
		write_text_inv(leftMargin,menuLine,text_str_display);
	else
		write_text(leftMargin,menuLine,text_str_display);

	// 9
	menuLine+=2;
	write_text(leftMargin,menuLine,text_str_sound);
	if ((mainMenu_sound==0)&&((c!=8)||(bb)))
		write_text_inv(tabstop1,menuLine,text_str_off);
	else
		write_text(tabstop1,menuLine,text_str_off);

	if ((mainMenu_sound==1)&&((c!=8)||(bb)))
		write_text_inv(tabstop3,menuLine,text_str_fast);
	else
		write_text(tabstop3,menuLine,text_str_fast);

	if ((mainMenu_sound==2)&&((c!=8)||(bb)))
		write_text_inv(30,menuLine,text_str_accurate);
	else
		write_text(30,menuLine,text_str_accurate);

#ifdef CYCLONE
	// 10
	menuLine+=2;
	write_text(leftMargin,menuLine,text_str_ham);
	if ((mainMenu_ham==0)&&((c!=9)||(bb)))
		write_text_inv(tabstop3,menuLine,text_str_off);
	else
		write_text(tabstop3,menuLine,text_str_off);

	if ((mainMenu_ham==1)&&((c!=9)||(bb)))
		write_text_inv(tabstop5,menuLine,text_str_on);
	else
		write_text(tabstop5,menuLine,text_str_on);
#else
	// 10
	menuLine+=2;
	if ((c==9)&&(bb))
		write_text_inv(leftMargin,menuLine,text_str_savestates);
	else
		write_text(leftMargin,menuLine,text_str_savestates);
#endif

	// 11
	menuLine+=2;
	if ((c==10)&&(bb))
		write_text_inv(leftMargin,menuLine,"Custom Control Config (Y)");
	else
		write_text(leftMargin,menuLine,"Custom Control Config (Y)");

	// 12
	menuLine+=2;
	if ((c==11)&&(bb))
		write_text_inv(leftMargin,menuLine,"More Options (B)");
	else
		write_text(leftMargin,menuLine,"More Options (B)");

	menuLine++;
	write_text(leftMargin,menuLine,text_str_separator);

	// 13
	menuLine++;
	if ((c==12)&&(bb))
		write_text_inv(leftMargin,menuLine,text_str_reset);
	else
		write_text(leftMargin,menuLine,text_str_reset);

	menuLine++;
	write_text(leftMargin,menuLine,text_str_separator);

	// 14
	menuLine++;
	if ((c==13)&&(bb))
		write_text_inv(leftMargin,menuLine,"Save Config for current game");
	else
		write_text(leftMargin,menuLine,"Save Config for current game");

	/*menuLine+=2;
	if ((c==13)&&(bb))
		write_text_inv(leftMargin,menuLine,"Save Script for current game");
	else
		write_text(leftMargin,menuLine,"Save Script for current game");*/

	// 15
	menuLine+=2;
	if ((c==14)&&(bb))
		write_text_inv(leftMargin,menuLine,"Save General Config");
	else
		write_text(leftMargin,menuLine,"Save General Config");

	menuLine++;
	write_text(leftMargin,menuLine,text_str_separator);

	// 16
	menuLine++;
	if ((c==15)&&(bb))
		write_text_inv(leftMargin,menuLine,text_str_exit);
	else
		write_text(leftMargin,menuLine,text_str_exit);

	menuLine++;
	write_text(leftMargin,menuLine,text_str_separator);

	text_flip();

	b++;
}

static void showWarning(char *msg)
{
		text_draw_window(54/7,91/8,255/7,64/8,"--- Config ---");
		write_text(12,14,msg);
		write_text(11,16,"Press any button to continue");
		text_flip();
		SDL_Event ev;
		SDL_Delay(333);
		while(SDL_PollEvent(&ev))
		SDL_Delay(10);
		while(!SDL_PollEvent(&ev))
				SDL_Delay(10);
		while(SDL_PollEvent(&ev))
				if (ev.type==SDL_QUIT)
		exit(1);
		SDL_Delay(200);

}

void update_display()
{
	char layersize[20];
	sprintf(layersize, "%dx480", screenWidth);
	setenv("SDL_OMAP_LAYER_SIZE",layersize,1);
	char bordercut[20];
	sprintf(bordercut, "%d,%d,0,%d", mainMenu_cutLeft, mainMenu_cutRight, 270-mainMenu_displayedLines);
	setenv("SDL_OMAP_BORDER_CUT",bordercut,1);
	prSDLScreen = SDL_SetVideoMode(320, 270, 16, SDL_SWSURFACE|SDL_FULLSCREEN|SDL_DOUBLEBUF);
}

static int key_mainMenu(int *cp)
{
	static int delay=0;
	int back_c = -1;
	int c=(*cp);
	int end=0;
	int left=0, right=0, up=0, down=0, hit0=0, hit1=0, hit2=0, hit3=0, hit4=0, hit5=0, hit6=0;
	SDL_Event event;
	int info=0;
	delay++;
	if (delay<5) return end;
	delay=0;
	SDL_Joystick *joy = uae4all_joy0 ;

#if !defined(GCW0)

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
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_X)) hit0=1;
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_B)) hit0=1;
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_START)) hit1=1;
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_SELECT)) hit2=1;
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_L)) hit3=1;
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_R)) hit4=1;
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_A)) hit5=1;
	else if (SDL_JoystickGetButton(joy, GP2X_BUTTON_Y)) hit6=1;

#else
	while (SDL_PollEvent(&event) > 0)
	{
		if (event.type == SDL_QUIT)
		{
			mainMenu_case=MAIN_MENU_CASE_REBOOT;
			end=-1;
		}
		else if (event.type == SDL_JOYBUTTONDOWN)
		{
			switch (event.jbutton.button)
			{

			case GP2X_BUTTON_RIGHT: right=1; break;
			case GP2X_BUTTON_LEFT: left=1; break;
			case GP2X_BUTTON_UP: up=1; break;
			case GP2X_BUTTON_DOWN: down=1; break;
			case GP2X_BUTTON_B: hit0=1; break;
			case GP2X_BUTTON_START: hit1=1; break;
			case GP2X_BUTTON_L: hit3=1; break;
			case GP2X_BUTTON_R: hit4=1; break;
			case GP2X_BUTTON_X: hit0=1; break;
			case GP2X_BUTTON_A: hit5=1; break;
			case GP2X_BUTTON_Y: hit6=1; break;
			case GP2X_BUTTON_SELECT: hit2=1; break;
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
			if (info) showInfo();

			else if (hit1)
			{
				mainMenu_case=MAIN_MENU_CASE_RUN;
				end=1;
			}
			else if (hit2)
			{
				mainMenu_case=MAIN_MENU_CASE_CANCEL;
				end=1;
			}
			else if (hit3)
			{
				mainMenu_case=MAIN_MENU_CASE_DISPLAY;
				end=1;
			}
			else if (hit4)
			{
				// reset
				back_c = c;
				hit0 = 1;
				c = 12;
			}
			else if (hit5)
			{
				// more options
				back_c = c;
				hit0 = 1;
				c = 11;
			}
			else if (hit6)
			{
				// custom controls
				back_c = c;
				hit0 = 1;
				c = 10;
			}
			else if (up)
			{
				if(nr_drives<2 && c==4)
					c=0;
				else if(nr_drives<3 && c==4)
					c=1;
				else if(nr_drives<4 && c==4)
					c=2;
				else
					c--;
				if (c < 0) c = 15;
			}
			else if (down)
			{
				if(nr_drives<4 && c==2)
					c=4;
				else if(nr_drives<3 && c==1)
					c=4;
				else if(nr_drives<2 && c==0)
					c=4;
				else
					c=(c+1)%16;
			}

	/* New Menu
	0 = DF0:
	1 = DF1:
	2 = DF2:
	3 = DF3:
	4 = eject all drives
	5 = number of drives
	6 = floppy speed
	7 = display settings
	8 = sound
	9 = savestates (fame&uae core) / ham emulation (cyclone)
	10 = custom controls
	11 = more options
	12 = reset
	13 = save config current game
	14 = save general config
	15 = exit
	*/
			switch(c)
			{
				case 0:
					if (hit0)
					{
						current_drive=0;
						mainMenu_case=MAIN_MENU_CASE_LOAD;
						end=1;
					}
					break;
				case 1:
					if (hit0)
					{
						current_drive=1;
						mainMenu_case=MAIN_MENU_CASE_LOAD;
						end=1;
					}
					break;
				case 2:
					if (hit0)
					{
						current_drive=2;
						mainMenu_case=MAIN_MENU_CASE_LOAD;
						end=1;
					}
					break;
				case 3:
					if (hit0)
					{
						current_drive=3;
						mainMenu_case=MAIN_MENU_CASE_LOAD;
						end=1;
					}
					break;
				case 4:
					// eject
					if (hit0)
					{
						strcpy(uae4all_image_file0, "");
						strcpy(uae4all_image_file1, "");
						strcpy(uae4all_image_file2, "");
						strcpy(uae4all_image_file3, "");
						//mainMenu_case=MAIN_MENU_CASE_EJECT;
						//end=3;
					}
					break;
				case 5:
					if (left)
					{
						if (nr_drives>1)
							nr_drives--;
						else
							nr_drives=4;
					}
					else if (right)
					{
						if (nr_drives<4)
							nr_drives++;
						else
							nr_drives=1;
					}
					break;
				case 6:
					if (left)
					{
						if (mainMenu_floppyspeed>100)
							mainMenu_floppyspeed/=2;
						else
							mainMenu_floppyspeed=800;
					}
					else if (right)
					{
						if (mainMenu_floppyspeed<800)
							mainMenu_floppyspeed*=2;
						else
							mainMenu_floppyspeed=100;
					}
					break;
				case 7:
					if (hit0)
					{
						mainMenu_case=MAIN_MENU_CASE_DISPLAY;
						end=1;
					}
					break;
				case 8:
					if (left)
					{
						if (mainMenu_sound == 1)
							mainMenu_sound = 0;
						else if (mainMenu_sound == 2)
							mainMenu_sound = 1;
						else if (mainMenu_sound == 0)
							mainMenu_sound = 2;
					}
					else if (right)
					{
						if (mainMenu_sound == 2)
							mainMenu_sound = 0;
						else if (mainMenu_sound == 0)
							mainMenu_sound = 1;
						else if (mainMenu_sound == 1)
							mainMenu_sound = 2;
					}
					break;
			#ifdef CYCLONE
				case 9:
					if (left || right)
					{
						if (mainMenu_ham)
							mainMenu_ham = 0;
						else
							mainMenu_ham = 1;
					}
					break;
			#else
				case 9:
					if (hit0)
					{
						mainMenu_case=MAIN_MENU_CASE_SAVESTATES;
						end=1;
					}
					break;
			#endif
				case 10:
					if (hit0)
					{
						mainMenu_case=MAIN_MENU_CASE_CONTROLS;
						end=1;
					}
					break;
				case 11:
					if (hit0)
					{
						mainMenu_case=MAIN_MENU_CASE_MISC;
						printf("Launch main menu MISC\n");
						end=1;
					}
					break;
				case 12:
					if (hit0)
					{
						mainMenu_case=MAIN_MENU_CASE_RESET;
						end=1;
					}
					break;
				case 13:
					if (hit0)
					{
						mainMenu_case=MAIN_MENU_CASE_SAVE;
						//end=1;
						if (saveconfig())
						 showWarning("Config saved for this game");
						//createScript();
					}
					break;
				case 14:
					if (hit0)
					{
						mainMenu_case=MAIN_MENU_CASE_SAVE;
						saveconfig(1);
						showWarning("General config file saved");
					}
					break;
				case 15:
					if (hit0)
					{
						mainMenu_case=MAIN_MENU_CASE_REBOOT;
						end=1;
					}
					break;
			}
			if (back_c>=0)
			{
				c=back_c;
				back_c=-1;
			}

	//}


	(*cp)=c;
	return end;
}

static void raise_mainMenu()
{
	setenv("SDL_OMAP_LAYER_SIZE","640x480",1);
	setenv("SDL_OMAP_BORDER_CUT","0,0,0,30",1);
#ifndef GCW0
	prSDLScreen = SDL_SetVideoMode(320, 270, 16, SDL_SWSURFACE|SDL_FULLSCREEN|SDL_DOUBLEBUF);
#else
	prSDLScreen = SDL_SetVideoMode(prSDLScreen->w, prSDLScreen->h, 16, SDL_SWSURFACE|SDL_FULLSCREEN|SDL_DOUBLEBUF);
#endif
	int i;

	text_draw_background();
	text_flip();
	for(i=0;i<10;i++)
	{
		text_draw_background();
		text_draw_window(40,(10-i)*24,260,200,text_str_title);
		text_flip();
	}
}
/*
static void refreshDrives(int &old_drive)
{

	if (old_drive<mainMenu_drives) return;
	else
	{
		int temp=mainMenu_drives;
		mainMenu_drives=old_drive;
		for(int i=temp+1;i<=old_drive;i++)
		{
			printf("Refreshing drive %d\n",i-1);
			changed_df[i-1][0]=0;
			real_changed_df[i-1]=1;
			if (i==2) uae4all_image_file1[0]=0;
			else if (i==3) uae4all_image_file2[0]=0;
			else if (i==4) uae4all_image_file3[0]=0;
			disk_eject(i-1);
		}
		mainMenu_drives=temp;
	}

}
*/
static void unraise_mainMenu()
{
	int i;

	for(i=9;i>=0;i--)
	{
		text_draw_background();
		text_draw_window(40,(10-i)*24,260,200,text_str_title);
		text_flip();
	}
	text_draw_background();
	text_flip();
}

int saveAdfDir()
{
	char path[300];
	sprintf(path, "%s/conf/adfdir.conf", launchDir);
	FILE *f=fopen(path,"w");
	if (!f) return 0;
	char buffer[310];
	sprintf((char*)buffer, "path=%s\n",currentDir);
	fputs(buffer,f);
	fclose(f);
	return 1;
}

void setCpuSpeed()
{
#if !defined(GCW0)
	if(mainMenu_cpuSpeed!=lastCpuSpeed)
	{
		sprintf((char*)speedCmd, "echo %d > /proc/pandora/cpu_mhz_max", mainMenu_cpuSpeed);
		system(speedCmd);
		lastCpuSpeed	= mainMenu_cpuSpeed;
	}
	if(mainMenu_ntsc!=ntsc)
	{
		ntsc=mainMenu_ntsc;
		if(ntsc)
			system("sudo /usr/pandora/scripts/op_lcdrate.sh 60");
		else
			system("sudo /usr/pandora/scripts/op_lcdrate.sh 50");
	}
#endif
	update_display();
}

int run_mainMenu()
{
#if defined(AUTO_RUN) || defined(AUTO_FRAMERATE) || defined(AUTO_PROFILER)
	return 1;
#else
#if !defined(DEBUG_UAE4ALL) && !defined(PROFILER_UAE4ALL)
	static int c=0;
#else
	static int c=1;
#endif
	int end;
	int old_sound_rate = sound_rate;
	mainMenu_case=-1;
#if !defined(PANDORA) && !defined(GCW0)
	setBatteryLED(0);
#endif

	// reinit text
	init_text(0);

	while(mainMenu_case<0)
	{
		raise_mainMenu();
		end=0;
		draw_mainMenu(c);

		while(!end)
		{
			draw_mainMenu(c);
			end=key_mainMenu(&c);
		}
		unraise_mainMenu();

		switch(mainMenu_case)
		{
		case MAIN_MENU_CASE_LOAD:
			if(run_menuLoad(currentDir)) loadconfig();
			mainMenu_case=-1;
			break;
		case MAIN_MENU_CASE_SAVESTATES:
			run_menuSavestates();
			if(savestate_state == STATE_DORESTORE || savestate_state == STATE_DOSAVE)
				mainMenu_case=1;
			else
				mainMenu_case=-1;
			break;
		case MAIN_MENU_CASE_EJECT:
			mainMenu_case=3;
			break;
		case MAIN_MENU_CASE_CANCEL:
			if (emulating)
			{
				setCpuSpeed();
				mainMenu_case=1;
			}
			else
				mainMenu_case=-1;
			break;
		case MAIN_MENU_CASE_RESET:
			setCpuSpeed();
			gp2xMouseEmuOn=0;
			gp2xButtonRemappingOn=0;

			mainMenu_drives=nr_drives;
			if (kickstart!=oldkickstart)
				{
					oldkickstart=kickstart;
					sprintf(romfile,"%s/kickstarts/%s",launchDir,kickstarts_rom_names[kickstart]);
					//strcpy(romfile,kickstarts_rom_names[kickstart-1]);
					bReloadKickstart=1;
					uae4all_init_rom(romfile);
				}
			if (emulating)
			{
				mainMenu_case=2;

				break;
			}
		case MAIN_MENU_CASE_RUN:
			setCpuSpeed();
			mainMenu_case=1;
			break;
		case MAIN_MENU_CASE_CONTROLS:
			{
				run_menuControls();
				mainMenu_case=-1;
			}
			break;
		case MAIN_MENU_CASE_DISPLAY:
			{
				run_menuDisplay();
				mainMenu_case=-1;
			}
			break;
		case MAIN_MENU_CASE_MISC:
			{
				//int olddrives=mainMenu_drives;
				run_menuMisc();
				//if (mainMenu_drives!=olddrives)
				//refreshDrives(olddrives);
				mainMenu_case=-1;
			}
			break;
//		case MAIN_MENU_CASE_SAVE:
//			saveconfig();
//			break;
		case MAIN_MENU_CASE_REBOOT:
#ifndef NO_SOUND
			gp2x_stop_sound();
#endif
			saveAdfDir();
			SDL_Quit();
			if (gp2xClockSpeed != -1)
			{
				// try to set it back to something vaguely sane
#if !defined(PANDORA) && !defined(GCW0)
#ifdef WIZ
				setGP2XClock(533);
#else
				setGP2XClock(200);
#endif
#endif
			}
			sync();

			if (!norelaunchGP2XMenu)
			{
				chdir("/usr/gp2x");
				execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
			}
			exit(0);
			break;
		default:
			mainMenu_case=-1;
		}
	}

	if (sound_rate != old_sound_rate)
		init_sound();

	return mainMenu_case;
#endif
}

typedef struct _cmdline_opt
{
	char *optname;
	int  len; // if 0, it means int
	void *opt;
} cmdline_opt;

static cmdline_opt cmdl_opts[] =
{
	{ "-norelaunchmenu",  0, &norelaunchGP2XMenu },
	{ "-statusln",        0, &mainMenu_showStatus },
	{ "-mousemultiplier", 0, &mainMenu_mouseMultiplier },
	{ "-sound",           0, &mainMenu_sound },
	{ "-soundrate",       0, &sound_rate },
	{ "-autosave",        0, &mainMenu_autosave },
	{ "-systemclock",     0, &mainMenu_throttle },
	{ "-syncthreshold",   0, &timeslice_mode },
	{ "-frameskip",       0, &mainMenu_frameskip },
	{ "-skipintro",       0, &skipintro },
	{ "-ntsc",            0, &mainMenu_ntsc },
	{ "-joyconf",            0, &mainMenu_joyConf },
	{ "-use1mbchip",            0, &mainMenu_chipMemory },
	{ "-autofire",            0, &mainMenu_autofire },
	{ "-drives",            0, &mainMenu_drives },
	{ "-script",            0, &mainMenu_enableScripts},
	{ "-screenshot",            0, &mainMenu_enableScreenshots},
	{ "-scaling",            0, &enableHWscaling},
	{ "-kick",            sizeof(romfile), romfile },
	{ "-df0",             sizeof(uae4all_image_file0), uae4all_image_file0 },
	{ "-df1",             sizeof(uae4all_image_file1), uae4all_image_file1 },
	{ "-df2",             sizeof(uae4all_image_file2), uae4all_image_file2 },
	{ "-df3",             sizeof(uae4all_image_file2), uae4all_image_file3 },
};
static void replace(char * str,char replace, char toreplace)
{
	while(*str)
	{
		if (*str==toreplace) *str=replace;
		str++;
	}
}

int saveconfig(int general)
{
	char path[300];
	char *p;
	if (general) sprintf(path, "%s/conf/uaeconfig.conf", launchDir);
	else{
		if (!uae4all_image_file0[0]) return 0;
	p=uae4all_image_file0+strlen(uae4all_image_file0)-1;
	while (*p != '/')p--;
	p++;
	int len=strlen(p);
	char gamename[len];
	strcpy(gamename,p);

	char * pch;
	pch = strstr (gamename,".adf.gz");
	if (pch) strcpy(pch,"\0");
		else{
				pch = strstr (gamename,".adf");
				if (pch) strcpy(pch,"\0");
		}
	if (uae4all_image_file0[0])	sprintf(path, "%s/conf/%s.conf", launchDir,gamename);
	else return 0;
	}

	FILE *f=fopen(path,"w");
	if (!f) return 0;
	char buffer[255];

	sprintf((char*)buffer, "kickstart=%d\n",kickstart);
	fputs(buffer,f);

	sprintf((char*)buffer, "scaling=%d\n",enableHWscaling);
	fputs(buffer,f);

	sprintf((char*)buffer, "showstatus=%d\n",mainMenu_showStatus);
	fputs(buffer,f);
	sprintf((char*)buffer, "mousemultiplier=%d\n",mainMenu_mouseMultiplier);
	fputs(buffer,f);
	sprintf((char*)buffer, "systemclock=%d\n",mainMenu_throttle);
	fputs(buffer,f);
	sprintf((char*)buffer, "syncthreshold=%d\n",timeslice_mode);
	fputs(buffer,f);
	sprintf((char*)buffer, "frameskip=%d\n",mainMenu_frameskip);
	fputs(buffer,f);
	sprintf((char*)buffer, "sound=%d\n",mainMenu_sound);
	fputs(buffer,f);
	sprintf((char*)buffer, "soundrate=%d\n",sound_rate);
	fputs(buffer,f);
	sprintf((char*)buffer, "autosave=%d\n",mainMenu_autosave);
	fputs(buffer,f);
	sprintf((char*)buffer, "gp2xclock=%d\n",gp2xClockSpeed);
	fputs(buffer,f);
	sprintf((char*)buffer, "joyconf=%d\n",mainMenu_joyConf);
	fputs(buffer,f);

	sprintf((char*)buffer, "autofire=%d\n",mainMenu_autofire);
	fputs(buffer,f);

	sprintf((char*)buffer, "stylusOffset=%d\n",mainMenu_stylusOffset);
	fputs(buffer,f);
	sprintf((char*)buffer, "tapDelay=%d\n",mainMenu_tapDelay);
	fputs(buffer,f);
	sprintf((char*)buffer, "scanlines=%d\n",mainMenu_scanlines);
	fputs(buffer,f);
	sprintf((char*)buffer, "ham=%d\n",mainMenu_ham);
	fputs(buffer,f);
	sprintf((char*)buffer, "enableScreenshots=%d\n",mainMenu_enableScreenshots);
	fputs(buffer,f);
	sprintf((char*)buffer, "floppyspeed=%d\n",mainMenu_floppyspeed);
	fputs(buffer,f);

	sprintf((char*)buffer, "drives=%d\n",nr_drives);
	fputs(buffer,f);

	sprintf((char*)buffer, "videomode=%d\n",mainMenu_ntsc);
	fputs(buffer,f);
	sprintf((char*)buffer, "mainMenu_chipMemory=%d\n",mainMenu_chipMemory);
	fputs(buffer,f);
	sprintf((char*)buffer, "mainMenu_bogoMemory=%d\n",mainMenu_bogoMemory);
	fputs(buffer,f);

	sprintf((char*)buffer, "mainMenu_cpuSpeed=%d\n",mainMenu_cpuSpeed);
	fputs(buffer,f);

	sprintf((char*)buffer, "presetModeId=%d\n",presetModeId);
	fputs(buffer,f);
	sprintf((char*)buffer, "moveX=%d\n",moveX);
	fputs(buffer,f);
	sprintf((char*)buffer, "moveY=%d\n",moveY);
	fputs(buffer,f);
	sprintf((char*)buffer, "displayedLines=%d\n",mainMenu_displayedLines);
	fputs(buffer,f);
	sprintf((char*)buffer, "screenWidth=%d\n",screenWidth);
	fputs(buffer,f);
	sprintf((char*)buffer, "cutLeft=%d\n",mainMenu_cutLeft);
	fputs(buffer,f);
	sprintf((char*)buffer, "cutRight=%d\n",mainMenu_cutRight);
	fputs(buffer,f);

	sprintf((char*)buffer, "customControls=%d\n",mainMenu_customControls);
	fputs(buffer,f);
	sprintf((char*)buffer, "custom_dpad=%d\n",mainMenu_custom_dpad);
	fputs(buffer,f);
	sprintf((char*)buffer, "custom_up=%d\n",mainMenu_custom_up);
	fputs(buffer,f);
	sprintf((char*)buffer, "custom_down=%d\n",mainMenu_custom_down);
	fputs(buffer,f);
	sprintf((char*)buffer, "custom_left=%d\n",mainMenu_custom_left);
	fputs(buffer,f);
	sprintf((char*)buffer, "custom_right=%d\n",mainMenu_custom_right);
	fputs(buffer,f);
	sprintf((char*)buffer, "custom_A=%d\n",mainMenu_custom_A);
	fputs(buffer,f);
	sprintf((char*)buffer, "custom_B=%d\n",mainMenu_custom_B);
	fputs(buffer,f);
	sprintf((char*)buffer, "custom_X=%d\n",mainMenu_custom_X);
	fputs(buffer,f);
	sprintf((char*)buffer, "custom_Y=%d\n",mainMenu_custom_Y);
	fputs(buffer,f);
	sprintf((char*)buffer, "custom_L=%d\n",mainMenu_custom_L);
	fputs(buffer,f);
	sprintf((char*)buffer, "custom_R=%d\n",mainMenu_custom_R);
	fputs(buffer,f);

	if(!general)
	{
		char namebuffer[128];
		strcpy(namebuffer,uae4all_image_file0);
		replace (namebuffer,'|',' ');
		sprintf((char*)buffer, "df0=%s\n",namebuffer);
		fputs(buffer,f);
		if (uae4all_image_file1[0])
		{
			strcpy(namebuffer,uae4all_image_file1);
			replace (namebuffer,'|',' ');
			sprintf((char*)buffer, "df1=%s\n",namebuffer);
			fputs(buffer,f);
		}
		if (uae4all_image_file2[0])
		{
			strcpy(namebuffer,uae4all_image_file2);
			replace (namebuffer,'|',' ');
			sprintf((char*)buffer, "df2=%s\n",namebuffer);
			fputs(buffer,f);
		}
		if (uae4all_image_file3[0])
		{
			strcpy(namebuffer,uae4all_image_file3);
			replace (namebuffer,'|',' ');
			sprintf((char*)buffer, "df3=%s\n",namebuffer);
			fputs(buffer,f);
		}
	}
	else
	{
		sprintf((char*)buffer, "script=%d\n",mainMenu_enableScripts);
		fputs(buffer,f);

		sprintf((char*)buffer, "screenshot=%d\n",mainMenu_enableScreenshots);
		fputs(buffer,f);

		sprintf((char*)buffer, "skipintro=%d\n",skipintro);
		fputs(buffer,f);
	}
	fclose(f);
	return 1;
}

void loadconfig(int general)
{
	if(general)
	{
		char path1[300];
		sprintf(path1, "%s/conf/adfdir.conf", launchDir);
		FILE *f1=fopen(path1,"rt");
		if(!f1)
		{
			printf ("No config file %s!\n",path1);
			strcpy(currentDir, launchDir);
			strcat(currentDir, "/roms/");
		}
		else
		{
			fscanf(f1,"path=%s\n",&currentDir);
			fclose(f1);
		}
	}

	char path[300];
	char *p;

	if (general)sprintf(path, "%s/conf/uaeconfig.conf", launchDir);
	else
	{
		p=uae4all_image_file0+strlen(uae4all_image_file0)-1;
		while (*p != '/') p--;
		p++;
		int len=strlen(p);
		char gamename[len];
		strcpy(gamename,p);
		char * pch;
		pch = strstr (gamename,".adf");
		if (pch) *pch='\0';
		/*else{
				pch = strstr (gamename,".adf");
				if (pch) strcpy(pch,"\0");
	}
	*/
	sprintf(path, "%s/conf/%s.conf", launchDir,gamename);
//	printf("read config from file %s and path %s\n",gamename,path);

	}
	FILE *f=fopen(path,"rt");
	if (!f){
		printf ("No config file %s!\n",path);
		return;
	}
	char filebuffer[128];
	fscanf(f,"kickstart=%d\n",&kickstart);
	fscanf(f,"scaling=%d\n",&enableHWscaling);
	fscanf(f,"showstatus=%d\n",&mainMenu_showStatus);
	fscanf(f,"mousemultiplier=%d\n",&mainMenu_mouseMultiplier );
	fscanf(f,"systemclock=%d\n",&mainMenu_throttle);
	fscanf(f,"syncthreshold=%d\n", &timeslice_mode);
	fscanf(f,"frameskip=%d\n",&mainMenu_frameskip);
	fscanf(f,"sound=%d\n",&mainMenu_sound );
	fscanf(f,"soundrate=%d\n",&sound_rate);
	fscanf(f,"autosave=%d\n",&mainMenu_autosave);
	fscanf(f,"gp2xclock=%d\n", &gp2xClockSpeed);
	fscanf(f,"joyconf=%d\n",&mainMenu_joyConf);
	fscanf(f,"autofire=%d\n",&mainMenu_autofire);
	fscanf(f,"stylusOffset=%d\n",&mainMenu_stylusOffset);
	fscanf(f,"tapDelay=%d\n",&mainMenu_tapDelay);
	fscanf(f,"scanlines=%d\n",&mainMenu_scanlines);
	fscanf(f,"ham=%d\n",&mainMenu_ham);
	fscanf(f,"enableScreenshots=%d\n",&mainMenu_enableScreenshots);
	fscanf(f,"floppyspeed=%d\n",&mainMenu_floppyspeed);
	//int olddrives=nr_drives;
	fscanf(f,"drives=%d\n",&nr_drives);
	fscanf(f,"videomode=%d\n",&mainMenu_ntsc);
	//if (mainMenu_drives!=olddrives) refreshDrives(olddrives);
	//fscanf(f,"mainMenu_useOneMegaChip=%d\n",&mainMenu_useOneMegaChip);
	fscanf(f,"mainMenu_chipMemory=%d\n",&mainMenu_chipMemory);
	fscanf(f,"mainMenu_bogoMemory=%d\n",&mainMenu_bogoMemory);
	fscanf(f,"mainMenu_cpuSpeed=%d\n",&mainMenu_cpuSpeed);
	fscanf(f,"presetModeId=%d\n",&presetModeId);
	switch(presetModeId)
	{
		case 0: strcpy(presetMode, "320x200 upscaled"); break;
		case 1: strcpy(presetMode, "320x216 upscaled"); break;
		case 2: strcpy(presetMode, "320x240 upscaled"); break;
		case 3: strcpy(presetMode, "320x256 upscaled"); break;
		case 4: strcpy(presetMode, "320x262 upscaled"); break;
		case 5: strcpy(presetMode, "320x270 upscaled"); break;
		case 6: strcpy(presetMode, "320x200 NTSC"); break;
		case 7: strcpy(presetMode, "320x200 fullscreen"); break;
	}
	fscanf(f,"moveX=%d\n",&moveX);
	fscanf(f,"moveY=%d\n",&moveY);
	fscanf(f,"displayedLines=%d\n",&mainMenu_displayedLines);
	fscanf(f,"screenWidth=%d\n",&screenWidth);
	fscanf(f,"cutLeft=%d\n",&mainMenu_cutLeft);
	fscanf(f,"cutRight=%d\n",&mainMenu_cutRight);
	fscanf(f,"customControls=%d\n",&mainMenu_customControls);
	fscanf(f,"custom_dpad=%d\n",&mainMenu_custom_dpad);
	fscanf(f,"custom_up=%d\n",&mainMenu_custom_up);
	fscanf(f,"custom_down=%d\n",&mainMenu_custom_down);
	fscanf(f,"custom_left=%d\n",&mainMenu_custom_left);
	fscanf(f,"custom_right=%d\n",&mainMenu_custom_right);
	fscanf(f,"custom_A=%d\n",&mainMenu_custom_A);
	fscanf(f,"custom_B=%d\n",&mainMenu_custom_B);
	fscanf(f,"custom_X=%d\n",&mainMenu_custom_X);
	fscanf(f,"custom_Y=%d\n",&mainMenu_custom_Y);
	fscanf(f,"custom_L=%d\n",&mainMenu_custom_L);
	fscanf(f,"custom_R=%d\n",&mainMenu_custom_R);

	if (!general){
	fscanf(f,"df0=%s\n",&filebuffer);
	replace(filebuffer,' ','|');
	strcpy(uae4all_image_file0,filebuffer);

	fscanf(f,"df1=%s\n",&filebuffer);
	replace(filebuffer,' ','|');
	strcpy(uae4all_image_file1,filebuffer);
	extractFileName(uae4all_image_file1,filename1);

	fscanf(f,"df2=%s\n",&filebuffer);
	replace(filebuffer,' ','|');
	strcpy(uae4all_image_file2,filebuffer);
	extractFileName(uae4all_image_file2,filename2);

	fscanf(f,"df3=%s\n",&filebuffer);
	replace(filebuffer,' ','|');
	strcpy(uae4all_image_file3,filebuffer);
	extractFileName(uae4all_image_file3,filename3);
	}
	else
	{
		fscanf(f,"script=%d\n",&mainMenu_enableScripts);
		fscanf(f,"screenshot=%d\n", &mainMenu_enableScreenshots);
		fscanf(f,"skipintro=%d\n", &skipintro);
		mainMenu_drives=nr_drives;
	}

	update_display();

	fclose(f);
	set_joyConf();
}

void parse_cmdline(int argc, char **argv)
{
	int arg, i, found;

	for (arg = 1; arg < argc-1; arg++)
	{
		for (i = found = 0; i < sizeof(cmdl_opts) / sizeof(cmdl_opts[0]); i++)
		{
			if (strcmp(argv[arg], cmdl_opts[i].optname) == 0)
			{
				arg++;
				if (cmdl_opts[i].len == 0)
					*(int *)(cmdl_opts[i].opt) = atoi(argv[arg]);
				else
				{
					strncpy((char *)cmdl_opts[i].opt, argv[arg], cmdl_opts[i].len);
					((char *)cmdl_opts[i].opt)[cmdl_opts[i].len-1] = 0;
				}
				found = 1;
				break;
			}
		}
		if (!found) printf("skipping unknown option: \"%s\"\n", argv[arg]);
	}
}
