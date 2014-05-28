#include<stdio.h>
#include<SDL.h>

extern SDL_Surface *prSDLScreen;
#if defined(PANDORA) || defined(GCW0)
extern SDL_Surface *prSDLScaleScreen;
#endif

#define MENU_FILE_SPLASH DATA_PREFIX "gp2xsplash.bmp"
#define MENU_FILE_BACKGROUND DATA_PREFIX "background.bmp"
#define MENU_FILE_WINDOW DATA_PREFIX "window.bmp"
#define MENU_FILE_TEXT DATA_PREFIX "text.bmp"
#ifdef DREAMCAST
#define MENU_DIR_DEFAULT "/cd/"
#else
#define MENU_DIR_DEFAULT "."
#endif

void text_draw_background();
void init_text(int splash);
void quit_text(void);
void write_text(int x, int y, char * str);
void write_text_inv(int x, int y, char * str);
void write_centered_text(int y, char * str);
void write_num(int x, int y, int v);
void write_num_inv(int x, int y, int v);
void text_draw_window(int x, int y, int w, int h, char *title);
void text_draw_barra(int x, int y, int w, int h, int per, int max);
void text_draw_window_bar(int x, int y, int w, int h, int per, int max, char *title);
void _text_draw_window(SDL_Surface *sf, int x, int y, int w, int h, char *title);
void _text_draw_window_bar(SDL_Surface *sf, int x, int y, int w, int h, int per, int max, char *title);
//void menuin_Effect();
//void menuout_Effect();
int save_thumb(int code,char *path);
int save_png(SDL_Surface* surface,char *path);

int createScript(int bIcon=0);

//int save_png(SDL_Surface* surface);
//void setupDrives(int num);
//void clearScreen();
//void fixDisplay();

// void text_draw_menu_msg();
void text_flip(void);
void set_joyConf(void);
void parse_cmdline(int argc, char **argv);
void loadconfig(int general=0);
int saveconfig(int general=0);

void drawPleaseWait(void);
void menu_raise(void);
void menu_unraise(void);

int run_mainMenu();
int run_menuLoad(char *);
int run_menuGame();
int run_menuSavestates();
int run_menuMisc();
int run_menuControls();
int run_menuDisplay();
void update_display();

int run_menuControl();
enum { SCREENSHOT, ICON };

enum { MAIN_MENU_CASE_REBOOT, MAIN_MENU_CASE_LOAD, MAIN_MENU_CASE_RUN, MAIN_MENU_CASE_RESET, MAIN_MENU_CASE_CANCEL, MAIN_MENU_CASE_SAVESTATES, MAIN_MENU_CASE_EJECT, MAIN_MENU_CASE_MISC, MAIN_MENU_CASE_SAVE, MAIN_MENU_CASE_CONTROLS, MAIN_MENU_CASE_DISPLAY};



#define DEFAULT_STATUSLN 0
#define DEFAULT_MOUSEMULTIPLIER 2
#define DEFAULT_SOUND 1
#define DEFAULT_AUTOSAVE 1
#define DEFAULT_SYSTEMCLOCK 0
#define DEFAULT_SYNCTHRESHOLD 2
#define DEFAULT_SKIPINTRO 1
#define DEFAULT_NTSC 0
#define DEFAULT_JOYCONF 2
#define DEFAULT_USE1MBCHIP 1
#define DEFAULT_AUTOFIRE 1
#define DEFAULT_DRIVES 4
#define DEFAULT_ENABLESCRIPTS 0
#define DEFAULT_ENABLESCREENSHOTS 0
#define DEFAULT_SCALING 0
#define DEFAULT_KICKSTART 1


extern void loadconfig(int general);