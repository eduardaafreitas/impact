#ifndef __PLAYER__
#define __PLAYER__
																						
#include "headers.h"
#include "bullet.h"
#include "enemy.h"

#define PATH_PLAYER "resources/player_ship/sheet.png"

typedef enum {standard, up, down, back, front} playerPose;

struct player_ship{
    ALLEGRO_BITMAP* sprites_player[5];
    playerPose atual_pose;
    int shot_flag, special_flag;
    float pos_x, pos_y;
    float speed;
    int score;
    int health_points;
    bullets *bullet;
    float time_since_last_shot;
    float shot_cooldown;
};
typedef struct player_ship player_ship;

player_ship *init_player();
void update_player(ALLEGRO_EVENT event, player_ship *player, enemy *enemy_active);
void draw_player(player_ship *player);
void free_player(player_ship *player);

void shoot_player(player_ship *player);
void update_bullets_player(player_ship *player, enemy *enemy_active);
void draw_bullets_player(player_ship *player);

#endif