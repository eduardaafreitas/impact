#ifndef __SCREEN__
#define __SCREEN__
																																																
#include "headers.h"
#include "player.h"

#define SIZE_X 960
#define SIZE_Y 720
#define FONT_TITLE_PATH "resources/fonts/Starjedi.ttf"
#define FONT_TEXT_PATH "resources/fonts/Segoe.ttf"
#define FONT_TEXT2_PATH "resources/fonts/SegoeBold.ttf"

void initial_text(ALLEGRO_FONT *font_text);
void animate_background(ALLEGRO_BITMAP* background, int* background_x, int speed);
void initial_animation(ALLEGRO_FONT *font_text, ALLEGRO_FONT *font_title);
void show_hp();
void draw_hud();
#endif