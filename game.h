#ifndef __GAME__
#define __GAME__

#include "headers.h"
#include "player.h"
#include "screen.h"

#define PATH_BG1 "resources/bg/bg1.png"
#define PATH_BG2 "resources/bg/bg2.png"
#define PATH_BG3 "resources/bg/bg3.png"

extern int wave_level;
extern float elapsed_time;

extern unsigned char key[ALLEGRO_KEY_MAX];

enum game_state {start, serving, playing, fase2, end_fase, end_game};
extern enum game_state state;

void inicia_allegro(bool teste, char *descricao);

void inicializando();

void state_start();
void state_serving();

void state_playing();
void state_end_game();
void state_pause();

void update_wave_level(player_ship* player);

void entry_identifyer(unsigned char *key, player_ship *player);
void manage_enemy_wave(enemy* enemy_wave, player_ship* player, ALLEGRO_FONT* font);

#endif