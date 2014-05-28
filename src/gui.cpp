 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Interface to the Tcl/Tk GUI
  *
  * Copyright 1996 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"
#include "config.h"
#include "uae.h"
#include "options.h"
#include "gui.h"
#include "menu.h"
#include "vkbd.h"
#include "debug_uae4all.h"
#include "custom.h"
#include "memory.h"
#include "xwin.h"
#include "drawing.h"
#include "sound.h"
#include "audio.h"
#include "keybuf.h"
#include "keyboard.h"
#include "disk.h"
#include "savestate.h"
#include "menu/menu.h"

#include <SDL.h>

//#include "pnd_io_evdev.h"
//#include "pnd_io_evdev.c"

#ifdef PROFILER_UAE4ALL
unsigned long long uae4all_prof_initial[UAE4ALL_PROFILER_MAX];
unsigned long long uae4all_prof_sum[UAE4ALL_PROFILER_MAX];
unsigned long long uae4all_prof_executed[UAE4ALL_PROFILER_MAX];
#endif

#ifdef DREAMCAST
#include <SDL_dreamcast.h>
#define VIDEO_FLAGS_INIT SDL_HWSURFACE|SDL_FULLSCREEN
#else
#ifdef PSP
#define VIDEO_FLAGS_INIT SDL_SWSURFACE
#else
#ifdef GIZMONDO
#define VIDEO_FLAGS_INIT SDL_HWSURFACE|SDL_FULLSCREEN
#else
#ifdef GCW0
#define VIDEO_FLAGS_INIT SDL_HWSURFACE
#else
#ifdef GP2X
#define VIDEO_FLAGS_INIT SDL_SWSURFACE|SDL_FULLSCREEN
#else
#define VIDEO_FLAGS_INIT SDL_HWSURFACE
#endif
#endif
#endif
#endif
#endif

#ifdef GP2X
#include "gp2x.h"
#include "gp2xutil.h"
extern int gp2xMouseEmuOn, gp2xButtonRemappingOn;
extern bool switch_autofire;
int justMovedUp=0, justMovedDown=0, justMovedLeft=0, justMovedRight=0;
int justLComma=0, justLPeriod=0, justLK=0;
int justPressedA=0, justPressedB=0, justPressedX=0, justPressedY=0;
int justPressedL=0, justPressedR=0;
int stylusClickOverride=0;
int stylusAdjustX=0, stylusAdjustY=0;
int gfxHeight = 0;
int hwScaled = 0;
int screenWidth = 640;
#endif

#ifdef PSP
#include "psp.h"
#include "psputil.h"
extern int gp2xMouseEmuOn, gp2xButtonRemappingOn;
#endif

#ifdef GIZMONDO
#include "gizmondo.h"
#include "gizmondoutil.h"
extern int gp2xMouseEmuOn, gp2xButtonRemappingOn;
#endif

#ifdef DOUBLEBUFFER
#define VIDEO_FLAGS VIDEO_FLAGS_INIT | SDL_DOUBLEBUF
#else
#define VIDEO_FLAGS VIDEO_FLAGS_INIT
#endif

static char _show_message_str[40]={
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

int show_message=0;
char *show_message_str=(char *)&_show_message_str[0];

extern SDL_Surface *prSDLScreen;

extern struct uae_prefs changed_prefs;
extern struct uae_prefs currprefs;
extern SDL_Joystick *uae4all_joy0, *uae4all_joy1;

extern int keycode2amiga(SDL_keysym *prKeySym);
extern int uae4all_keystate[];

int emulating=0;
char uae4all_image_file0[128]  = { 0, };
char uae4all_image_file1[128] = { 0, };
char uae4all_image_file2[128]  = { 0, };
char uae4all_image_file3[128] = { 0, };

int drawfinished=0;

int moved_x = 0;
int moved_y = 0;

extern int mainMenu_throttle, mainMenu_frameskip, mainMenu_sound, mainMenu_case, mainMenu_autosave, mainMenu_ntsc, mainMenu_drives, mainMenu_showStatus;
extern int mainMenu_customControls;
extern int mainMenu_custom_dpad;
extern int mainMenu_custom_up;
extern int mainMenu_custom_down;
extern int mainMenu_custom_left;
extern int mainMenu_custom_right;
extern int mainMenu_custom_A;
extern int mainMenu_custom_B;
extern int mainMenu_custom_X;
extern int mainMenu_custom_Y;
extern int mainMenu_custom_L;
extern int mainMenu_custom_R;

extern int mainMenu_displayedLines;
extern int presetModeId;
extern char presetMode[20];

static void getChanges(void)
{
    m68k_speed=mainMenu_throttle;
#ifndef NO_SOUND
    if (mainMenu_sound)
    {
#if defined (GP2X) || defined (PSP) || defined (GIZMONDO)
		if (mainMenu_sound == 1)
		{
			changed_produce_sound=2;
		}
		else
		{
			// accurate sound
			changed_produce_sound=3;
		}
	    sound_default_evtime();
#else
	    changed_produce_sound=2;
	    sound_default_evtime();
#endif
    }
    else
#endif
	    changed_produce_sound=0;
    changed_gfx_framerate=mainMenu_frameskip;
}

int gui_init (void)
{

//Se ejecuta justo despues del MAIN

    if (prSDLScreen==NULL)
#ifdef DREAMCAST
	prSDLScreen=SDL_SetVideoMode(320,240,16,VIDEO_FLAGS);
#else

#ifdef PANDORA
	if(hwScaled)
		prSDLScreen = SDL_SetVideoMode(320, gfxHeight, 16, VIDEO_FLAGS);
	else
	{
		prSDLScreen = SDL_CreateRGBSurface(SDL_SWSURFACE,320,240,16,0,0,0,0);
		prSDLScaleScreen = SDL_SetVideoMode(640, 480, 16, VIDEO_FLAGS);
	}
#else
	prSDLScreen=SDL_SetVideoMode(320,240,16,VIDEO_FLAGS);
#endif

#endif
    SDL_ShowCursor(SDL_DISABLE);
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);
    if (prSDLScreen!=NULL)
    {
	emulating=0;

#if !defined(DEBUG_UAE4ALL) && !defined(PROFILER_UAE4ALL) && !defined(AUTO_RUN) && !defined(AUTO_FRAMERATE)
	//uae4all_image_file[0]=0;
	//uae4all_image_file2[0]=0;
#else
	//strcpy(uae4all_image_file,"prueba.adz");
	//strcpy(uae4all_image_file2,"prueba2.adz");
#endif
	init_text(1);
	if (!uae4all_image_file0[0])
		run_mainMenu();
	quit_text();
	vkbd_init();

#ifdef GP2X
	inputmode_init();
#if !defined(PANDORA) && !defined(GCW0)
	volumecontrol_init();
#endif
#endif
printf("4\n");
#if defined (PSP) || defined (GIZMONDO)
	inputmode_init();
#endif

	uae4all_pause_music();
printf("5\n");
	emulating=1;
	getChanges();
printf("6\n");
	check_all_prefs();
printf("7\n");
	reset_frameskip();
printf("8\n");
#ifdef DEBUG_FRAMERATE
	uae4all_update_time();
#endif
#ifdef PROFILER_UAE4ALL
	uae4all_prof_init();
	uae4all_prof_add("M68K");			// 0
	uae4all_prof_add("EVENTS");			// 1
	uae4all_prof_add("HSync");			// 2
	uae4all_prof_add("Copper");			// 3
	uae4all_prof_add("Audio");			// 4
	uae4all_prof_add("CIA");			// 5
	uae4all_prof_add("Blitter");			// 6
	uae4all_prof_add("Vsync");			// 7
	uae4all_prof_add("update_fetch");		// 8
	uae4all_prof_add("linetoscr");			// 9
	uae4all_prof_add("do_long_fetch");		// 10
	uae4all_prof_add("pfield_doline");		// 11
	uae4all_prof_add("draw_sprites_ecs");		// 12
	uae4all_prof_add("flush_block");		// 13
	uae4all_prof_add("SET_INTERRUPT");		// 14
/*
	uae4all_prof_add("15");		// 15
	uae4all_prof_add("16");		// 16
	uae4all_prof_add("17");		// 17
	uae4all_prof_add("18");		// 18
	uae4all_prof_add("19");		// 19
	uae4all_prof_add("20");		// 20
	uae4all_prof_add("21");		// 21
	uae4all_prof_add("22");		// 22
*/
#endif
#ifdef DREAMCAST
	SDL_DC_EmulateKeyboard(SDL_FALSE);
#endif

	/*int test = pnd_evdev_open(pnd_evdev_nub1);
	printf("nub1 open: %d\n", test);
	test = pnd_evdev_open(pnd_evdev_nub2);
	printf("nub2 open: %d\n", test);*/

	return 0;
    }
    return -1;
}

int gui_update (void)
{
    extern char *savestate_filename;
    extern int saveMenu_n_savestate;
    //SE EJECUTA DESPUES DE INICIAR EL CORE 68k
    printf("gui_update\n");
    printf("uae4all_image_file0 %s\n", uae4all_image_file0);
    printf("uae4all_image_file1 %s\n", uae4all_image_file1);
    printf("uae4all_image_file2 %s\n", uae4all_image_file2);
    printf("uae4all_image_file3 %s\n", uae4all_image_file3);
    strcpy(changed_df[0],uae4all_image_file0);
    strcpy(changed_df[1],uae4all_image_file1);
    strcpy(changed_df[2],uae4all_image_file2);
    strcpy(changed_df[3],uae4all_image_file3);
    strcpy(savestate_filename,uae4all_image_file0);
    switch(saveMenu_n_savestate)
    {
	    case 1:
    		strcat(savestate_filename,"-1.asf"); break;
	    case 2:
    		strcat(savestate_filename,"-2.asf"); break;
	    case 3:
    		strcat(savestate_filename,"-3.asf"); break;
	    default:
    	   	strcat(savestate_filename,".asf");
    }
    real_changed_df[0]=1;
    real_changed_df[1]=1;
    real_changed_df[2]=1;
    real_changed_df[3]=1;
    return 0;
}


static void goMenu(void)
{

   int exitmode=0;
   int autosave=mainMenu_autosave;
   if (quit_program != 0)
	    return;
#ifdef PROFILER_UAE4ALL
   uae4all_prof_show();
#endif
#ifdef DEBUG_FRAMERATE
   uae4all_show_time();
#endif
   emulating=1;
   vkbd_quit();
   init_text(0);
   pause_sound();

   menu_raise();
   exitmode=run_mainMenu();
   notice_screen_contents_lost();
   resume_sound();
   if ((!(strcmp(prefs_df[0],uae4all_image_file0))) || ((!(strcmp(prefs_df[1],uae4all_image_file1)))))
	   menu_unraise();
   quit_text();
   vkbd_init();
#ifdef DREAMCAST
   SDL_DC_EmulateKeyboard(SDL_FALSE);
#endif
    getChanges();
	vkbd_init_button2();
    if (exitmode==1 || exitmode==2)
    {
    	    extern char *savestate_filename;
    	    extern int saveMenu_n_savestate;
	    for(int i=0;i<mainMenu_drives;i++)
	    {
		if (i==0 && strcmp(changed_df[0],uae4all_image_file0)) {
		    strcpy(changed_df[0],uae4all_image_file0);
		    real_changed_df[0]=1;
		}
		else if (i==1 && strcmp(changed_df[1],uae4all_image_file1)) {
		    strcpy(changed_df[1],uae4all_image_file1);
		    real_changed_df[1]=1;
		}
		else if (i==2 && strcmp(changed_df[2],uae4all_image_file2)) {
		    strcpy(changed_df[2],uae4all_image_file2);
		    real_changed_df[2]=1;
		}
		else if (i==3 && strcmp(changed_df[3],uae4all_image_file3)) {
		    strcpy(changed_df[3],uae4all_image_file3);
		    real_changed_df[3]=1;
		}
	    }
    	    strcpy(savestate_filename,uae4all_image_file0);
	    switch(saveMenu_n_savestate)
    	    {
	    	case 1:
    			strcat(savestate_filename,"-1.asf"); break;
	    	case 2:
    			strcat(savestate_filename,"-2.asf"); break;
	    	case 3:
    			strcat(savestate_filename,"-3.asf"); break;
	    	default:
    	   		strcat(savestate_filename,".asf");
    	    }
    }
    if (exitmode==3)
    {
    	    extern char *savestate_filename;
    	    extern int saveMenu_n_savestate;
	    for(int i=0;i<mainMenu_drives;i++)
	    {
		changed_df[i][0]=0;
		if (i==0) {
		    uae4all_image_file0[0]=0;
		    if (strcmp(changed_df[0],uae4all_image_file0))
		    {
			strcpy(changed_df[0],uae4all_image_file0);
			real_changed_df[0]=1;
		    }
		}
		else if (i==1) {
		    uae4all_image_file1[0]=0;
		    if (strcmp(changed_df[1],uae4all_image_file1))
		    {
			strcpy(changed_df[1],uae4all_image_file1);
			real_changed_df[1]=1;
		    }
		}
		else if (i==2) {
		    uae4all_image_file2[0]=0;
		    if (strcmp(changed_df[2],uae4all_image_file2))
		    {
			strcpy(changed_df[2],uae4all_image_file2);
			real_changed_df[2]=1;
		    }
		}
		else if (i==3) {
		    uae4all_image_file3[0]=0;
		    if (strcmp(changed_df[3],uae4all_image_file3))
		    {
			strcpy(changed_df[3],uae4all_image_file3);
			real_changed_df[3]=1;
		    }
		}
		disk_eject(i);
	    }
    	    strcpy(savestate_filename,uae4all_image_file0);
    	    switch(saveMenu_n_savestate)
    	    {
	   	case 1:
    			strcat(savestate_filename,"-1.asf"); break;
	    	case 2:
    			strcat(savestate_filename,"-2.asf"); break;
	    	case 3:
    			strcat(savestate_filename,"-3.asf"); break;
	    	default:
    	  	 	strcat(savestate_filename,".asf");
    	    }
    }
    if (exitmode==2)
    {
	    if (autosave!=mainMenu_autosave)
	    {
	    	prefs_df[0][0]=0;
	   		prefs_df[1][0]=0;
			prefs_df[2][0]=0;
	   		prefs_df[3][0]=0;
	    }
		if(gp2xButtonRemappingOn)
			togglemouse();
		int gp2xMouseEmuOn=0;
		int gp2xButtonRemappingOn=0;
	    uae_reset ();
    }
    check_all_prefs();
    gui_purge_events();
    notice_screen_contents_lost();
#ifdef DEBUG_FRAMERATE
    uae4all_update_time();
#endif
#ifdef PROFILER_UAE4ALL
    uae4all_prof_init();
#endif

}

int nowSuperThrottle=0, goingSuperThrottle=0, goingVkbd=0;

static void goSuperThrottle(void)
{
	if (!nowSuperThrottle)
	{
		nowSuperThrottle=1;
		m68k_speed=6;
		changed_produce_sound=0;
		changed_gfx_framerate=80;
		check_prefs_changed_cpu();
		check_prefs_changed_audio();
		check_prefs_changed_custom();
	}
}

static void leftSuperThrottle(void)
{
	if (nowSuperThrottle)
	{
		nowSuperThrottle=0;
		getChanges();
		check_prefs_changed_cpu();
		check_prefs_changed_audio();
		check_prefs_changed_custom();
	}
}

int customKey;
void getMapping(int customId)
{
	switch(customId)
	{
		case 1: customKey=AK_UP; break;
		case 2: customKey=AK_DN; break;
		case 3: customKey=AK_LF; break;
		case 4: customKey=AK_RT; break;
		case 5: customKey=AK_NP0; break;
		case 6: customKey=AK_NP1; break;
		case 7: customKey=AK_NP2; break;
		case 8: customKey=AK_NP3; break;
		case 9: customKey=AK_NP4; break;
		case 10: customKey=AK_NP5; break;
		case 11: customKey=AK_NP6; break;
		case 12: customKey=AK_NP7; break;
		case 13: customKey=AK_NP8; break;
		case 14: customKey=AK_NP9; break;
		case 15: customKey=AK_ENT; break;
		case 16: customKey=AK_NPDIV; break;
		case 17: customKey=AK_NPMUL; break;
		case 18: customKey=AK_NPSUB; break;
		case 19: customKey=AK_NPADD; break;
		case 20: customKey=AK_NPDEL; break;
		case 21: customKey=AK_NPLPAREN; break;
		case 22: customKey=AK_NPRPAREN; break;
		case 23: customKey=AK_SPC; break;
		case 24: customKey=AK_BS; break;
		case 25: customKey=AK_TAB; break;
		case 26: customKey=AK_RET; break;
		case 27: customKey=AK_ESC; break;
		case 28: customKey=AK_DEL; break;
		case 29: customKey=AK_LSH; break;
		case 30: customKey=AK_RSH; break;
		case 31: customKey=AK_CAPSLOCK; break;
		case 32: customKey=AK_CTRL; break;
		case 33: customKey=AK_LALT; break;
		case 34: customKey=AK_RALT; break;
		case 35: customKey=AK_LAMI; break;
		case 36: customKey=AK_RAMI; break;
		case 37: customKey=AK_HELP; break;
		case 38: customKey=AK_LBRACKET; break;
		case 39: customKey=AK_RBRACKET; break;
		case 40: customKey=AK_SEMICOLON; break;
		case 41: customKey=AK_COMMA; break;
		case 42: customKey=AK_PERIOD; break;
		case 43: customKey=AK_SLASH; break;
		case 44: customKey=AK_BACKSLASH; break;
		case 45: customKey=AK_QUOTE; break;
		case 46: customKey=AK_NUMBERSIGN; break;
		case 47: customKey=AK_LTGT; break;
		case 48: customKey=AK_BACKQUOTE; break;
		case 49: customKey=AK_MINUS; break;
		case 50: customKey=AK_EQUAL; break;
		case 51: customKey=AK_A; break;
		case 52: customKey=AK_B; break;
		case 53: customKey=AK_C; break;
		case 54: customKey=AK_D; break;
		case 55: customKey=AK_E; break;
		case 56: customKey=AK_F; break;
		case 57: customKey=AK_G; break;
		case 58: customKey=AK_H; break;
		case 59: customKey=AK_I; break;
		case 60: customKey=AK_J; break;
		case 61: customKey=AK_K; break;
		case 62: customKey=AK_L; break;
		case 63: customKey=AK_M; break;
		case 64: customKey=AK_N; break;
		case 65: customKey=AK_O; break;
		case 66: customKey=AK_P; break;
		case 67: customKey=AK_Q; break;
		case 68: customKey=AK_R; break;
		case 69: customKey=AK_S; break;
		case 70: customKey=AK_T; break;
		case 71: customKey=AK_U; break;
		case 72: customKey=AK_V; break;
		case 73: customKey=AK_W; break;
		case 74: customKey=AK_X; break;
		case 75: customKey=AK_Y; break;
		case 76: customKey=AK_Z; break;
		case 77: customKey=AK_1; break;
		case 78: customKey=AK_2; break;
		case 79: customKey=AK_3; break;
		case 80: customKey=AK_4; break;
		case 81: customKey=AK_5; break;
		case 82: customKey=AK_6; break;
		case 83: customKey=AK_7; break;
		case 84: customKey=AK_8; break;
		case 85: customKey=AK_9; break;
		case 86: customKey=AK_0; break;
		case 87: customKey=AK_F1; break;
		case 88: customKey=AK_F2; break;
		case 89: customKey=AK_F3; break;
		case 90: customKey=AK_F4; break;
		case 91: customKey=AK_F5; break;
		case 92: customKey=AK_F6; break;
		case 93: customKey=AK_F7; break;
		case 94: customKey=AK_F8; break;
		case 95: customKey=AK_F9; break;
		case 96: customKey=AK_F10;
	}
}

//int blubb=-1, test=0;
/*pnd_nubstate_t* t = new pnd_nubstate_t();
t->x=1;
t->y=1;*/
//pnd_nubstate_t t;
void gui_handle_events (void)
{
	printf("gui_handle_events\n");
/*if(blubb<0)
{
	test = pnd_evdev_open(pnd_evdev_nub1);
	printf("nub1 open: %d\n", test);
	test = pnd_evdev_open(pnd_evdev_nub2);
	printf("nub2 open: %d\n", test);
	t.x=1;
	t.y=1;
	printf("nubstate 1: x=%d y=%d\n", t.x, t.y);
}
blubb++;
if(blubb>100 && blubb<0)
{
	pnd_evdev_catchup(1);
	blubb=0;
}
test = pnd_evdev_nub_state(pnd_evdev_nub1, &t);
printf("nubstate 1: x=%d y=%d, returnvalue=%d\n", t.x, t.y, test);
pnd_evdev_nub_state(pnd_evdev_nub2, &t);
printf("nubstate 2: x=%d y=%d\n", t.x, t.y);
*/

#if ! defined (DREAMCAST) && ! defined (GP2X) && ! defined (PSP) && ! defined (GIZMONDO)
	Uint8 *keystate = SDL_GetKeyState(NULL);
	if ( keystate[SDLK_F12] )
		SDL_WM_ToggleFullScreen(prSDLScreen);
	else
	if ( keystate[SDLK_F11] )
#else
#ifdef DREAMCAST
	if (SDL_JoystickGetButton(uae4all_joy0,3) )
#endif
#ifdef GP2X
    if (SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_SELECT))
#endif
#ifdef PSP
	if (SDL_JoystickGetButton(uae4all_joy0, PSP_BUTTON_SELECT))
#endif
#ifdef GIZMONDO
	Uint8 *keystate = SDL_GetKeyState(NULL);
	if ( keystate[SDLK_F1] )
#endif
#endif
#ifdef GP2X
	{
		// hack: always use SDL_SWSURFACE in menus
		//switch_to_sw_sdl();
		goMenu();
		//switch_to_hw_sdl(0);
	}
#else
		goMenu();
#endif

#ifndef PANDORA
#if defined (DREAMCAST) || defined (GP2X) || defined (PSP) || defined (GIZMONDO)
#ifdef DREAMCAST
	if (SDL_JoystickGetAxis (uae4all_joy0, 2))
		if (vkbd_mode)
		{
			vkbd_mode=0;
   			notice_screen_contents_lost();
		}
	if (SDL_JoystickGetAxis (uae4all_joy0, 3))
		vkbd_mode=1;
#else

	if (!gp2xMouseEmuOn && !gp2xButtonRemappingOn)
	{
#ifdef GP2X
		if (SDL_JoystickGetButton (uae4all_joy0, GP2X_BUTTON_L))
#else
#ifdef GIZMONDO
		if (SDL_JoystickGetButton (uae4all_joy0, GIZ_LTRIG))
#else
		if (SDL_JoystickGetButton (uae4all_joy0, PSP_BUTTON_L))
#endif
#endif
		{
			/*if (vkbd_mode)
			{
				vkbd_mode = 0;
				goingVkbd=0;
				notice_screen_contents_lost();
			}
			else*/
				goingSuperThrottle=1;
		}
		else
		{
			if (!nowSuperThrottle)
				goingSuperThrottle=0;
			/*else
				goingVkbd=0;*/
		}

#ifdef GP2X
		if (SDL_JoystickGetButton (uae4all_joy0, GP2X_BUTTON_R))
#else
#ifdef GIZMONDO
		if (SDL_JoystickGetButton (uae4all_joy0, GIZ_RTRIG))
#else
		if (SDL_JoystickGetButton (uae4all_joy0, PSP_BUTTON_R))
#endif
#endif
		{
			if (goingSuperThrottle)
				goSuperThrottle();
			/*else
			{
				if (goingVkbd>4)
					vkbd_mode=1;
				else
					goingVkbd++;
			}*/
		}
		else
		{
			if (nowSuperThrottle)
				leftSuperThrottle();
			/*else
				goingVkbd=0;*/
		}
	}

#endif
#endif

	/*if (vkbd_key)
	{
		if (vkbd_keysave==-1234567)
		{
			SDL_keysym ks;
			ks.sym=vkbd_key;
			vkbd_keysave=keycode2amiga(&ks);
			if (vkbd_keysave >= 0)
			{
				if (!uae4all_keystate[vkbd_keysave])
				{
					uae4all_keystate[vkbd_keysave]=1;
					record_key(vkbd_keysave<<1);
				}
			}
		}
	}
	else
		if (vkbd_keysave!=-1234567)
		{
			if (vkbd_keysave >= 0)
			{
				uae4all_keystate[vkbd_keysave]=0;
				record_key((vkbd_keysave << 1) | 1);
			}
			vkbd_keysave=-1234567;
		}*/
#endif

#ifdef PANDORA
	Uint8 *keystate = SDL_GetKeyState(NULL);
	if(keystate[SDLK_F12])
	{
		if(hwScaled)
			SDL_WM_ToggleFullScreen(prSDLScreen);
		else
			SDL_WM_ToggleFullScreen(prSDLScaleScreen);
	}

if(!vkbd_mode)
{
	//L + R
	if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_L) && SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_R))
	{
		//up
		if(SDL_JoystickGetAxis(uae4all_joy0, 1) < 0)
		{
			moveVertical(1);
			moved_y += 2;
		}
		//down
		else if(SDL_JoystickGetAxis(uae4all_joy0, 1) > 0)
		{
			moveVertical(-1);
			moved_y -= 2;
		}
		//left
		else if(SDL_JoystickGetAxis(uae4all_joy0, 0) < 0)
		{
			screenWidth -=10;
			if(screenWidth<200)
				screenWidth = 200;
			update_display();
		}
		//right
		else if(SDL_JoystickGetAxis(uae4all_joy0, 0) > 0)
		{
			screenWidth +=10;
			if(screenWidth>800)
				screenWidth = 800;
			update_display();
		}
		//1
		else if(keystate[SDLK_1])
		{
			mainMenu_displayedLines = 200;
			screenWidth = 768;
			presetModeId = 0;
			strcpy(presetMode, "320x200 upscaled");
			update_display();
		}
		//2
		else if(keystate[SDLK_2])
		{
			mainMenu_displayedLines = 216;
			screenWidth = 716;
			presetModeId = 1;
			strcpy(presetMode, "320x216 upscaled");
			update_display();
		}
		//3
		else if(keystate[SDLK_3])
		{
			mainMenu_displayedLines = 240;
			screenWidth = 640;
			presetModeId = 2;
			strcpy(presetMode, "320x240 upscaled");
			update_display();
		}
		//4
		else if(keystate[SDLK_4])
		{
			mainMenu_displayedLines = 256;
			screenWidth = 600;
			presetModeId = 3;
			strcpy(presetMode, "320x256 upscaled");
			update_display();
		}
		//5
		else if(keystate[SDLK_5])
		{
			mainMenu_displayedLines = 262;
			screenWidth = 588;
			presetModeId = 4;
			strcpy(presetMode, "320x262 upscaled");
			update_display();
		}
		//6
		else if(keystate[SDLK_6])
		{
			mainMenu_displayedLines = 270;
			screenWidth = 570;
			presetModeId = 5;
			strcpy(presetMode, "320x270 upscaled");
			update_display();
		}
		//7
		else if(keystate[SDLK_7])
		{
			mainMenu_displayedLines = 200;
			screenWidth = 640;
			presetModeId = 6;
			strcpy(presetMode, "320x200 NTSC");
			update_display();
		}
		//8
		else if(keystate[SDLK_8])
		{
			mainMenu_displayedLines = 200;
			screenWidth = 800;
			presetModeId = 7;
			strcpy(presetMode, "320x200 fullscreen");
			update_display();
		}
	}

	//autofire on/off
	if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_R))
	{
		//(Y) button
		if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_Y))
		{
			if(!justPressedY)
			{
				//autofire on/off
				switch_autofire = !switch_autofire;
				justPressedY=1;
			}
		}
		else if(justPressedY)
			justPressedY=0;
	}

	if (mainMenu_customControls && !gp2xMouseEmuOn && !gp2xButtonRemappingOn)
	{
		if(mainMenu_custom_dpad == 0)
		{
			//UP
			if(SDL_JoystickGetAxis(uae4all_joy0, 1) < 0)
			{
				if(!justMovedUp)
				{
					if(mainMenu_custom_up == -1) buttonstate[0]=1;
					else if(mainMenu_custom_up == -2) buttonstate[2]=1;
					else if(mainMenu_custom_up > 0)
					{
						getMapping(mainMenu_custom_up);
						uae4all_keystate[customKey] = 1;
						record_key(customKey << 1);
					}
					justMovedUp=1;
				}
			}
			else if(justMovedUp)
			{
				if(mainMenu_custom_up == -1) buttonstate[0]=0;
				else if(mainMenu_custom_up == -2) buttonstate[2]=0;
				else if(mainMenu_custom_up > 0)
				{
					getMapping(mainMenu_custom_up);
					uae4all_keystate[customKey] = 0;
					record_key((customKey << 1) | 1);
				}
				justMovedUp=0;
			}

			//DOWN
			if(SDL_JoystickGetAxis(uae4all_joy0, 1) > 0)
			{
				if(!justMovedDown)
				{
					if(mainMenu_custom_down == -1) buttonstate[0]=1;
					else if(mainMenu_custom_down == -2) buttonstate[2]=1;
					else if(mainMenu_custom_down > 0)
					{
						getMapping(mainMenu_custom_down);
						uae4all_keystate[customKey] = 1;
						record_key(customKey << 1);
					}
					justMovedDown=1;
				}
			}
			else if(justMovedDown)
			{
				if(mainMenu_custom_down == -1) buttonstate[0]=0;
				else if(mainMenu_custom_down == -2) buttonstate[2]=0;
				else if(mainMenu_custom_down > 0)
				{
					getMapping(mainMenu_custom_down);
					uae4all_keystate[customKey] = 0;
					record_key((customKey << 1) | 1);
				}
				justMovedDown=0;
			}

			//LEFT
			if(SDL_JoystickGetAxis(uae4all_joy0, 0) < 0)
			{
				if(!justMovedLeft)
				{
					if(mainMenu_custom_left == -1) buttonstate[0]=1;
					else if(mainMenu_custom_left == -2) buttonstate[2]=1;
					else if(mainMenu_custom_left > 0)
					{
						getMapping(mainMenu_custom_left);
						uae4all_keystate[customKey] = 1;
						record_key(customKey << 1);
					}
					justMovedLeft=1;
				}
			}
			else if(justMovedLeft)
			{
				if(mainMenu_custom_left == -1) buttonstate[0]=0;
				else if(mainMenu_custom_left == -2) buttonstate[2]=0;
				else if(mainMenu_custom_left > 0)
				{
					getMapping(mainMenu_custom_left);
					uae4all_keystate[customKey] = 0;
					record_key((customKey << 1) | 1);
				}
				justMovedLeft=0;
			}

 			//RIGHT
			if(SDL_JoystickGetAxis(uae4all_joy0, 0) > 0)
			{
				if(!justMovedRight)
				{
					if(mainMenu_custom_right == -1) buttonstate[0]=1;
					else if(mainMenu_custom_right == -2) buttonstate[2]=1;
					else if(mainMenu_custom_right > 0)
					{
						getMapping(mainMenu_custom_right);
						uae4all_keystate[customKey] = 1;
						record_key(customKey << 1);
					}
					justMovedRight=1;
				}
			}
			else if(justMovedRight)
			{
				if(mainMenu_custom_right == -1) buttonstate[0]=0;
				else if(mainMenu_custom_right == -2) buttonstate[2]=0;
				else if(mainMenu_custom_right > 0)
				{
					getMapping(mainMenu_custom_right);
					uae4all_keystate[customKey] = 0;
					record_key((customKey << 1) | 1);
				}
				justMovedRight=0;
			}
		}

		//(A)
		if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_B))
		{
			if(!justPressedA)
			{
				if(mainMenu_custom_A == -1) buttonstate[0]=1;
				else if(mainMenu_custom_A == -2) buttonstate[2]=1;
				else if(mainMenu_custom_A > 0)
				{
					getMapping(mainMenu_custom_A);
					uae4all_keystate[customKey] = 1;
					record_key(customKey << 1);
				}
				justPressedA=1;
			}
		}
		else if(justPressedA)
		{
			if(mainMenu_custom_A == -1) buttonstate[0]=0;
			else if(mainMenu_custom_A == -2) buttonstate[2]=0;
			else if(mainMenu_custom_A > 0)
			{
				getMapping(mainMenu_custom_A);
				uae4all_keystate[customKey] = 0;
				record_key((customKey << 1) | 1);
			}
			justPressedA=0;
		}

		//(B)
		if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_A))
		{
			if(!justPressedB)
			{
				if(mainMenu_custom_B == -1) buttonstate[0]=1;
				else if(mainMenu_custom_B == -2) buttonstate[2]=1;
				else if(mainMenu_custom_B > 0)
				{
					getMapping(mainMenu_custom_B);
					uae4all_keystate[customKey] = 1;
					record_key(customKey << 1);
				}
				justPressedB=1;
			}
		}
		else if(justPressedB)
		{
			if(mainMenu_custom_B == -1) buttonstate[0]=0;
			else if(mainMenu_custom_B == -2) buttonstate[2]=0;
			else if(mainMenu_custom_B > 0)
			{
				getMapping(mainMenu_custom_B);
				uae4all_keystate[customKey] = 0;
				record_key((customKey << 1) | 1);
			}
			justPressedB=0;
		}

		//(X)
		if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_X))
		{
			if(!justPressedX)
			{
				if(mainMenu_custom_X == -1) buttonstate[0]=1;
				else if(mainMenu_custom_X == -2) buttonstate[2]=1;
				else if(mainMenu_custom_X > 0)
				{
					getMapping(mainMenu_custom_X);
					uae4all_keystate[customKey] = 1;
					record_key(customKey << 1);
				}
				justPressedX=1;
			}
		}
		else if(justPressedX)
		{
			if(mainMenu_custom_X == -1) buttonstate[0]=0;
			else if(mainMenu_custom_X == -2) buttonstate[2]=0;
			else if(mainMenu_custom_X > 0)
			{
				getMapping(mainMenu_custom_X);
				uae4all_keystate[customKey] = 0;
				record_key((customKey << 1) | 1);
			}
			justPressedX=0;
		}

		//(Y)
		if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_Y))
		{
			if(!justPressedY)
			{
				if(mainMenu_custom_Y == -1) buttonstate[0]=1;
				else if(mainMenu_custom_Y == -2) buttonstate[2]=1;
				else if(mainMenu_custom_Y > 0)
				{
					getMapping(mainMenu_custom_Y);
					uae4all_keystate[customKey] = 1;
					record_key(customKey << 1);
				}
				justPressedY=1;
			}
		}
		else if(justPressedY)
		{
			if(mainMenu_custom_Y == -1) buttonstate[0]=0;
			else if(mainMenu_custom_Y == -2) buttonstate[2]=0;
			else if(mainMenu_custom_Y > 0)
			{
				getMapping(mainMenu_custom_Y);
				uae4all_keystate[customKey] = 0;
				record_key((customKey << 1) | 1);
			}
			justPressedY=0;
		}

		//(L)
		if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_L))
		{
			if(!justPressedL)
			{
				if(mainMenu_custom_L == -1) buttonstate[0]=1;
				else if(mainMenu_custom_L == -2) buttonstate[2]=1;
				else if(mainMenu_custom_L > 0)
				{
					getMapping(mainMenu_custom_L);
					uae4all_keystate[customKey] = 1;
					record_key(customKey << 1);
				}
				justPressedL=1;
			}
		}
		else if(justPressedL)
		{
			if(mainMenu_custom_L == -1) buttonstate[0]=0;
			else if(mainMenu_custom_L == -2) buttonstate[2]=0;
			else if(mainMenu_custom_L > 0)
			{
				getMapping(mainMenu_custom_L);
				uae4all_keystate[customKey] = 0;
				record_key((customKey << 1) | 1);
			}
			justPressedL=0;
		}

		//(R)
		if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_R))
		{
			if(!justPressedR)
			{
				if(mainMenu_custom_R == -1) buttonstate[0]=1;
				else if(mainMenu_custom_R == -2) buttonstate[2]=1;
				else if(mainMenu_custom_R > 0)
				{
					getMapping(mainMenu_custom_R);
					uae4all_keystate[customKey] = 1;
					record_key(customKey << 1);
				}
				justPressedR=1;
			}
		}
		else if(justPressedR)
		{
			if(mainMenu_custom_R == -1) buttonstate[0]=0;
			else if(mainMenu_custom_R == -2) buttonstate[2]=0;
			else if(mainMenu_custom_R > 0)
			{
				getMapping(mainMenu_custom_R);
				uae4all_keystate[customKey] = 0;
				record_key((customKey << 1) | 1);
			}
			justPressedR=0;
		}
	}
	else if(!gp2xMouseEmuOn)
	{
		//DPad = arrow keys in stylus-mode
		if(gp2xButtonRemappingOn)
		{
			//dpad up
			if (SDL_JoystickGetAxis(uae4all_joy0, 1) < 0)
			{
				if(!justMovedUp)
				{
					//left and right mouse-buttons down
					buttonstate[0] = 1;
					buttonstate[2] = 1;
					stylusClickOverride = 1;
					justMovedUp=1;
				}
			}
			else if(justMovedUp)
			{
				//left and right mouse-buttons up
				buttonstate[0] = 0;
				buttonstate[2] = 0;
				stylusClickOverride = 0;
				justMovedUp=0;
			}
			//dpad down
			if (SDL_JoystickGetAxis(uae4all_joy0, 1) > 0)
			{
				if(!justMovedDown)
				{
					//no clicks with stylus now
					stylusClickOverride=1;
					justMovedDown=1;
				}
			}
			else if(justMovedDown)
			{
				//clicks active again
				stylusClickOverride=0;
				justMovedDown=0;
			}
			//dpad left
			if (SDL_JoystickGetAxis(uae4all_joy0, 0) < 0)
			{
				if(!justMovedLeft)
				{
					//left mouse-button down
					buttonstate[0] = 1;
					stylusClickOverride = 1;
					justMovedLeft=1;
				}
			}
			else if(justMovedLeft)
			{
				//left mouse-button up
				buttonstate[0] = 0;
				stylusClickOverride = 0;
				justMovedLeft=0;
			}
			//dpad right
			if (SDL_JoystickGetAxis(uae4all_joy0, 0) > 0)
			{
				if(!justMovedRight)
				{
					//right mouse-button down
					buttonstate[2] = 1;
					stylusClickOverride = 1;
					justMovedRight=1;
				}
			}
			else if(justMovedRight)
			{
				//right mouse-button up
				buttonstate[2] = 0;
				stylusClickOverride = 0;
				justMovedRight=0;
			}
			//L + up
			if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_L) && SDL_JoystickGetAxis(uae4all_joy0, 1) < 0)
				stylusAdjustY-=2;
			//L + down
			if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_L) && SDL_JoystickGetAxis(uae4all_joy0, 1) > 0)
				stylusAdjustY+=2;
			//L + left
			if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_L) && SDL_JoystickGetAxis(uae4all_joy0, 0) < 0)
				stylusAdjustX-=2;
			//L + right
			if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_L) && SDL_JoystickGetAxis(uae4all_joy0, 0) > 0)
				stylusAdjustX+=2;
		}
		//R-trigger in joystick mode
		else if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_R))
		{
			//(A) button
			if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_A))
			{
				if(!justPressedA)
				{
					//CTRL
					uae4all_keystate[AK_CTRL] = 1;
					record_key(AK_CTRL << 1);
					justPressedA=1;
				}
			}
			else if(justPressedA)
			{
				uae4all_keystate[AK_CTRL] = 0;
				record_key((AK_CTRL << 1) | 1);
				justPressedA=0;
			}
			//(B) button
			if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_B))
			{
				if(!justPressedB)
				{
					//left ALT
					uae4all_keystate[AK_LALT] = 1;
					record_key(AK_LALT << 1);
					justPressedB=1;
				}
			}
			else if(justPressedB)
			{
				uae4all_keystate[AK_LALT] = 0;
				record_key((AK_LALT << 1) | 1);
				justPressedB=0;
			}
			//(X) button
			if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_X))
			{
				if(!justPressedX)
				{
					//HELP
					uae4all_keystate[AK_HELP] = 1;
					record_key(AK_HELP << 1);
					justPressedX=1;
				}
			}
			else if(justPressedX)
			{
				//HELP
				uae4all_keystate[AK_HELP] = 0;
				record_key((AK_HELP << 1) | 1);
				justPressedX=0;
			}
		}
		else if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_L))
		{
			//(A) button
			if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_A))
			{
				if(!justPressedA)
				{
					//right mouse-button down
					buttonstate[2] = 1;
					justPressedA=1;
				}
			}
			else if(justPressedA)
			{
				//right mouse-button up
				buttonstate[2] = 0;
				justPressedA=0;
			}
			//(B) button
			if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_B))
			{
				if(!justPressedB)
				{
					//left mouse-button down
					buttonstate[0] = 1;
					justPressedB=1;
				}
			}
			else if(justPressedB)
			{
				//left mouse-button up
				buttonstate[0] = 0;
				justPressedB=0;
			}
		}
		else
		{
			if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_Y))
			{
				if(!justPressedY)
				{
					//SPACE
					uae4all_keystate[AK_SPC] = 1;
					record_key(AK_SPC << 1);
					justPressedY=1;
				}
			}
			else if(justPressedY)
			{
				//SPACE
				uae4all_keystate[AK_SPC] = 0;
				record_key((AK_SPC << 1) | 1);
				justPressedY=0;
			}
		}
	}
	else
	{
		if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_A))
		{
			if(!justPressedA)
			{
				//right mouse-button down
				buttonstate[2] = 1;
				justPressedA=1;
			}
		}
		else if(justPressedA)
		{
			//right mouse-button up
			buttonstate[2] = 0;
			justPressedA=0;
		}
		//(B) button
		if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_B))
		{
			if(!justPressedB)
			{
				//left mouse-button down
				buttonstate[0] = 1;
				justPressedB=1;
			}
		}
		else if(justPressedB)
		{
			//left mouse-button up
			buttonstate[0] = 0;
			justPressedB=0;
		}
		if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_Y))
		{
			if(!justPressedY)
			{
				//SPACE
				uae4all_keystate[AK_SPC] = 1;
				record_key(AK_SPC << 1);
				justPressedY=1;
			}
		}
		else if(justPressedY)
		{
			//SPACE
			uae4all_keystate[AK_SPC] = 0;
			record_key((AK_SPC << 1) | 1);
			justPressedY=0;
		}
		if (SDL_JoystickGetAxis(uae4all_joy0, 0) < 0)
		{
			if(!justMovedLeft)
			{
				//left ALT
				uae4all_keystate[0x64] = 1;
				record_key(0x64 << 1);
				justMovedLeft=1;
			}
		}
		else if(justMovedLeft)
		{
			//left ALT
			uae4all_keystate[0x64] = 0;
			record_key((0x64 << 1) | 1);
			justMovedLeft=0;
		}
		if (SDL_JoystickGetAxis(uae4all_joy0, 0) > 0)
		{
			if(!justMovedRight)
			{
				//left ALT
				uae4all_keystate[0x64] = 1;
				record_key(0x64 << 1);
				justMovedRight=1;
			}
		}
		else if(justMovedRight)
		{
			//left ALT
			uae4all_keystate[0x64] = 0;
			record_key((0x64 << 1) | 1);
			justMovedRight=0;
		}
	}

	if(!mainMenu_customControls && SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_R))
	{
		//R+dpad = arrow keys in joystick mode
		//dpad up
		if (SDL_JoystickGetAxis(uae4all_joy0, 1) < 0)
		{
			if(!justMovedUp)
			{
				//arrow up
				uae4all_keystate[0x4C] = 1;
				record_key(0x4C << 1);
				justMovedUp=1;
			}
		}
		else if(justMovedUp)
		{
			//arrow up
			uae4all_keystate[0x4C] = 0;
			record_key((0x4C << 1) | 1);
			justMovedUp=0;
		}
		//dpad down
		if (SDL_JoystickGetAxis(uae4all_joy0, 1) > 0)
		{
			if(!justMovedDown)
			{
				//arrow down
				uae4all_keystate[0x4D] = 1;
				record_key(0x4D << 1);
				justMovedDown=1;
			}
		}
		else if(justMovedDown)
		{
			//arrow down
			uae4all_keystate[0x4D] = 0;
			record_key((0x4D << 1) | 1);
			justMovedDown=0;
		}
		//dpad left
		if (SDL_JoystickGetAxis(uae4all_joy0, 0) < 0)
		{
			if(!justMovedLeft)
			{
				//arrow left
				uae4all_keystate[0x4F] = 1;
				record_key(0x4F << 1);
				justMovedLeft=1;
			}
		}
		else if(justMovedLeft)
		{
			//arrow left
			uae4all_keystate[0x4F] = 0;
			record_key((0x4F << 1) | 1);
			justMovedLeft=0;
		}
		//dpad right
		if (SDL_JoystickGetAxis(uae4all_joy0, 0) > 0)
		{
			if(!justMovedRight)
			{
				//arrow right
				uae4all_keystate[0x4E] = 1;
				record_key(0x4E << 1);
				justMovedRight=1;
			}
		}
		else if(justMovedRight)
		{
			//arrow right
			uae4all_keystate[0x4E] = 0;
			record_key((0x4E << 1) | 1);
			justMovedRight=0;
		}
	}

	//L+D: show/hide status ln
	if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_L) && keystate[SDLK_d])
	{
		keystate[SDLK_d]=0;
		mainMenu_showStatus = !mainMenu_showStatus;
	}
}

	//L+K: virtual keyboard
	if (SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_L) && keystate[SDLK_k])
	{
		if(!justLK)
		{
			vkbd_mode = !vkbd_mode;
			justLK=1;
		}
	}
	else if(justLK)
		justLK=0;

#ifndef CYCLONE
	if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_L) && keystate[SDLK_s])
	{
		keystate[SDLK_s]=0;
		savestate_state = STATE_DOSAVE;
	}
	if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_L) && keystate[SDLK_l])
	{
		extern char *savestate_filename;
		FILE *f=fopen(savestate_filename, "rb");
		keystate[SDLK_l]=0;
		if(f)
		{
			fclose(f);
			savestate_state = STATE_DORESTORE;
		}
		else
			gui_set_message("Failed: Savestate not found", 100);
	}
#endif
//if(SDL_JoystickGetButton(uae4all_joy0, GP2X_BUTTON_L) && keystate[SDLK_1])
//{
//	resChange = 1;
//	gfxHeight	 = 256;
//	prSDLScreen = SDL_SetVideoMode(320, 256, 16, VIDEO_FLAGS);
//	system("export SDL_OMAP_LAYER_SIZE=800x480");
//}
#endif

	if (vkbd_key)
	{
		if (vkbd_keysave==-1234567)
		{
			SDL_keysym ks;
			ks.sym=vkbd_key;
			vkbd_keysave=keycode2amiga(&ks);
			if (vkbd_keysave >= 0)
			{
				if (!uae4all_keystate[vkbd_keysave])
				{
					uae4all_keystate[vkbd_keysave]=1;
					record_key(vkbd_keysave<<1);
				}
			}
		}
	}
	else
		if (vkbd_keysave!=-1234567)
		{
			if (vkbd_keysave >= 0)
			{
				uae4all_keystate[vkbd_keysave]=0;
				record_key((vkbd_keysave << 1) | 1);
			}
			vkbd_keysave=-1234567;
		}
}

void gui_changesettings (void)
{
	dbg("GUI: gui_changesettings");
}

void gui_update_gfx (void)
{
// ANTES DE LA ENTRADA EN VIDEO
//	dbg("GUI: gui_update_gfx");
}

/*
int run_menuGame() {}
int run_menuControl() {}
*/

void gui_set_message(char *msg, int t)
{
	return;

	show_message=t;
	strncpy(show_message_str, msg, 36);
}

void gui_show_window_bar(int per, int max, int case_title)
{
	return;

	char *title;
	if (case_title)
		title="  Restore State";
	else
		title="  Save State";
	_text_draw_window_bar(prSDLScaleScreen,80,64,172,48,per,max,title);
#if defined(DOUBLEBUFFER) || defined(DINGOO)
	SDL_Flip(prSDLScaleScreen);
#endif
}

#ifdef PROFILER_UAE4ALL

static unsigned uae4all_prof_total_initial=0;
unsigned uae4all_prof_total=0;
static char *uae4all_prof_msg[UAE4ALL_PROFILER_MAX];

void uae4all_prof_init(void)
{
	unsigned i;
#ifndef DREAMCAST
	unsigned long long s=SDL_GetTicks();
#else
	unsigned long long s=timer_us_gettime64();
#endif
	for(i=0;i<UAE4ALL_PROFILER_MAX;i++)
	{
		uae4all_prof_initial[i]=s;
		uae4all_prof_sum[i]=0;
		uae4all_prof_executed[i]=0;
		if (!uae4all_prof_total)
			uae4all_prof_msg[i]=NULL;
	}
	uae4all_prof_total_initial=s;
}

void uae4all_prof_add(char *msg)
{
	if (uae4all_prof_total<UAE4ALL_PROFILER_MAX)
	{
		uae4all_prof_msg[uae4all_prof_total]=msg;
		uae4all_prof_total++;
	}
}

void uae4all_prof_show(void)
{
	unsigned i;
	double toper=0;
#ifndef DREAMCAST
	unsigned long long to=SDL_GetTicks()-uae4all_prof_total_initial;
#else
	unsigned long long to=uae4all_prof_sum[0]+uae4all_prof_sum[1];
	for(i=0;i<uae4all_prof_total;i++)
		if (uae4all_prof_sum[i]>to)
			uae4all_prof_sum[i]=0;
#endif

	puts("\n\n\n\n");
	puts("--------------------------------------------");
	for(i=0;i<uae4all_prof_total;i++)
	{
		unsigned long long t0=uae4all_prof_sum[i];
		double percent=(double)t0;
		percent*=100.0;
		percent/=(double)to;
		toper+=percent;
#ifdef DREAMCAST
		t0/=1000;
#endif
		printf("%s: %.2f%% -> Ticks=%i -> %iK veces\n",uae4all_prof_msg[i],percent,((unsigned)t0),(unsigned)(uae4all_prof_executed[i]>>10));
	}
	printf("TOTAL: %.2f%% -> Ticks=%i\n",toper,to);
	puts("--------------------------------------------"); fflush(stdout);
}
#endif
