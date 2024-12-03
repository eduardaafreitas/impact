#ifndef __ENEMY__
#define __ENEMY__

#include "headers.h"
#include "player.h"

#define ENEMY1_PATH "resources/enemy/enemy1.png"

//Tipos de inimigos:
// 1: apenas voa
// 2: atira sem mira
// 3: atira mirando
// 4: atira mais vezes
// 5: boss 1
// 6: boss 2

struct enemy{
    ALLEGRO_BITMAP* sprite;
    int shot_flag, special_flag;
    float pos_x, pos_y;
    float speed;
    int health_points;
    int type;
    bullets *bullet;

};
typedef struct enemy enemy;

enemy *init_enemy(ALLEGRO_BITMAP* sheet);
void spawn_enemy(enemy* enemy_active);
void update_enemy(enemy* enemy_active);
void draw_enemy(enemy *enemy_active);

#endif