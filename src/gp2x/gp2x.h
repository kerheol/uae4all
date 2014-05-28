#ifndef _GP2X_H
#define _GP2X_H

#define JOY_DEADZONE 500

#if defined(PANDORA)
//#define GP2X_BUTTON_UP              (0)
//#define GP2X_BUTTON_DOWN            (4)
//#define GP2X_BUTTON_LEFT            (2)
//#define GP2X_BUTTON_RIGHT           (6)
//#define GP2X_BUTTON_UPLEFT          (1)
//#define GP2X_BUTTON_UPRIGHT         (7)
//#define GP2X_BUTTON_DOWNLEFT        (3)
//#define GP2X_BUTTON_DOWNRIGHT       (5)
#define GP2X_BUTTON_Y			0
#define GP2X_BUTTON_A			1
#define GP2X_BUTTON_X			2
#define GP2X_BUTTON_B			3
#define GP2X_BUTTON_R			8
#define GP2X_BUTTON_L			7
#define GP2X_BUTTON_START		5
#define GP2X_BUTTON_SELECT		4
#define GP2X_BUTTON_CLICK		9 // doesnt seem to work, though it was the pandora button

#else // GP2X

#define GP2X_BUTTON_UP              (0)
#define GP2X_BUTTON_DOWN            (4)
#define GP2X_BUTTON_LEFT            (2)
#define GP2X_BUTTON_RIGHT           (6)
#define GP2X_BUTTON_UPLEFT          (1)
#define GP2X_BUTTON_UPRIGHT         (7)
#define GP2X_BUTTON_DOWNLEFT        (3)
#define GP2X_BUTTON_DOWNRIGHT       (5)
#define GP2X_BUTTON_CLICK           (18)
#define GP2X_BUTTON_A               (12)
#define GP2X_BUTTON_B               (13)
#define GP2X_BUTTON_X               (14)
#define GP2X_BUTTON_Y               (15)
#define GP2X_BUTTON_L               (10)
#define GP2X_BUTTON_R               (11)
#define GP2X_BUTTON_START           (8)
#define GP2X_BUTTON_SELECT          (9)
#define GP2X_BUTTON_VOLUP           (16)
#define GP2X_BUTTON_VOLDOWN         (17)

#endif
/*
enum  { GP2X_UP=0x1,       GP2X_LEFT=0x4,       GP2X_DOWN=0x10,  GP2X_RIGHT=0x40,
        GP2X_START=1<<8,   GP2X_SELECT=1<<9,    GP2X_L=1<<10,    GP2X_R=1<<11,
        GP2X_A=1<<12,      GP2X_B=1<<13,        GP2X_X=1<<14,    GP2X_Y=1<<15,
        GP2X_VOL_UP=1<<23, GP2X_VOL_DOWN=1<<22, GP2X_PUSH=1<<27 };
*/

#define GP2X_UP                              (1<<0)
#define GP2X_UP_LEFT                         (1<<1)
#define GP2X_LEFT                            (1<<2)
#define GP2X_DOWN_LEFT                       (1<<3)
#define GP2X_DOWN                            (1<<4)
#define GP2X_DOWN_RIGHT                      (1<<5)
#define GP2X_RIGHT                           (1<<6)
#define GP2X_UP_RIGHT                        (1<<7)
#define GP2X_A                              (1<<12)
#define GP2X_B                              (1<<13)
#define	GP2X_X				(1<<15)
#define	GP2X_Y				(1<<14)
#define	GP2X_L				(1<<10)
#define	GP2X_R				(1<<11)
#define	GP2X_SELECT				(1<<9)
#define	GP2X_START				(1<<8)
#define	GP2X_VOL_UP				(1<<16)
#define	GP2X_VOL_DOWN	       		(1<<17)
#define GP2X_PUSH			(1<<27)


//const int MOUSE_SCALE = 1;
#endif
