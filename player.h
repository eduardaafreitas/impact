#ifndef __PLAYER__
#define __PLAYER__
																						
#include "headers.h"
#include "screen.h"

#define PATH_PLAYER "resources/player_ship/sheet.png"
#define MAX_BULLETS 10

typedef enum {standard, up, down, back, front} playerPose;

struct bullets{
    float pos_x, pos_y;
    float speed;
    bool active;
};
typedef struct bullets bullets;

struct player_ship{
    ALLEGRO_BITMAP* sprites_player[5];
    playerPose atual_pose;
    int shot_flag, special_flag;
    float pos_x, pos_y;
    float speed;
    int score;
    int health_points;

    bullets bullet[MAX_BULLETS];
};
typedef struct player_ship player_ship;

void init_bullet(bullets *bullet);
player_ship *init_player();
void update_player(ALLEGRO_EVENT event, player_ship *player);
void draw_player(player_ship *player);

void shoot(player_ship *player);
void update_bullets(player_ship *player);
void draw_bullets(player_ship *player);

#endif