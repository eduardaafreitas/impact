#ifndef __ENEMY__
#define __ENEMY__

#include "headers.h"
#include "player.h"

#define ENEMY1_PATH "resources/enemy/enemy1.png"
#define ENEMY2_PATH "resources/enemy/enemy2.png"
#define ENEMY3_PATH "resources/enemy/enemy3.png"
#define ENEMY4_PATH "resources/enemy/enemy4.png"
#define BOSS1_PATH "resources/enemy/boss1.png"
#define ENEMY6_PATH "resources/enemy/enemy6.png"
#define ENEMY7_PATH "resources/enemy/enemy7.png"
#define ENEMY8_PATH "resources/enemy/enemy8.png"
#define ENEMY9_PATH "resources/enemy/enemy9.png"
#define BOSS2_PATH "resources/enemy/boss2.png"

//Tipos de inimigos:
// 1: apenas voa
// 2: atira sem mira
// 3: atira mirando
// 4: atira mais vezes
// 5: boss 1
// 6: apenas voa

struct enemy{
    ALLEGRO_BITMAP* sprite;
    ALLEGRO_BITMAP* spriteboss1[8];
    int shot_flag, special_flag;
    float pos_x, pos_y;
    float speed;
    int health_points;
    int type;
    bullets *bullet;
    float shoot_timer;
    bool can_shoot;
    bool boss;

};
typedef struct enemy enemy;

enemy *init_enemy(ALLEGRO_BITMAP* sheet, int type);
void spawn_enemy(enemy* enemy_active, bool boss);
void spawn_boss(enemy *enemy_active);
void update_enemy(enemy* enemy_active);
void shoot_multiple(enemy* enemy_active);
void draw_enemy(enemy *enemy_active);
void shoot_enemy(enemy *enemy_active);
void draw_bullets_enemy(enemy* enemy_active);
void update_bullets_enemy(bullets *bullet);

#endif