#include "sprites.h"
#include "screen.h"
#include "game.h"
#include "enemy.h"

enemy *init_enemy(ALLEGRO_BITMAP* sheet, int type){
    enemy *enemy_active;
    enemy_active = malloc(sizeof(enemy));
    if (enemy_active == NULL){
        printf("Erro ao alocar memoria (enemy_active)\n");
        exit(1);
    }
    enemy_active->pos_x = SIZE_X;
    enemy_active->pos_y = SIZE_Y;
    enemy_active->speed = 5.0;
    enemy_active->health_points = 0;
    enemy_active->type = type;
    enemy_active->shoot_timer = 0.0; // Inicializa o temporizador
    enemy_active->can_shoot = (type == 2 || type == 3 || type == 4 || type == 5 || type == 7 || type == 8 || type == 9 || type == 10); 
    sprites_enemy(sheet, enemy_active, type);
    enemy_active->bullet = alloc_bullets(MAX_BULLETS);
    enemy_active->boss = false;
    return enemy_active;
}

void spawn_enemy(enemy *enemy_active, bool boss) {
    enemy_active->pos_x = SIZE_X;
    if (enemy_active->type == 5){
        enemy_active->pos_y = (SIZE_Y - al_get_bitmap_height(enemy_active->sprite));
        enemy_active->speed = 0.5; // Aumenta a velocidade com o nível
        enemy_active->health_points = 10; // Aumenta os pontos de vida com o nível

    } else if(enemy_active->type == 10){
        enemy_active->pos_y = (0 + al_get_bitmap_height(enemy_active->sprite));
        enemy_active->speed = 0.5; // Aumenta a velocidade com o nível
        enemy_active->health_points = 10; // Aumenta os pontos de vida com o nível
    } else {
        enemy_active->pos_y = rand() % (SIZE_Y - al_get_bitmap_height(enemy_active->sprite));
        enemy_active->speed = 2.0 + (wave_level * 0.1); // Aumenta a velocidade com o nível
        enemy_active->health_points = 3 + wave_level / 5; // Aumenta os pontos de vida com o nível
    }
}

void update_enemy(enemy* enemy_active) {
    // Move o inimigo
    if (enemy_active == NULL) {
        return;
    }

    enemy_active->pos_x -= enemy_active->speed;
    if (enemy_active->can_shoot) {
        enemy_active->shoot_timer += 1.0 / 120.0; // Incrementa o temporizador
        if (enemy_active->shoot_timer >= 1.0) { // Verifica o intervalo de disparo
            shoot_enemy(enemy_active);
            enemy_active->shoot_timer = 0.0; // Reseta o temporizador
        }
    }
    if (enemy_active->pos_x < 0 || enemy_active->health_points <= 0) {
        if (enemy_active->type == 5 || enemy_active->type == 10){
            spawn_enemy(enemy_active, true);
        } else {
            spawn_enemy(enemy_active, false);
        }
    }
}

void draw_enemy(enemy *enemy_active){
    al_draw_bitmap(enemy_active->sprite, enemy_active->pos_x, enemy_active->pos_y, 0);
}

void shoot_enemy(enemy *enemy_active) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!enemy_active->bullet[i].active) {
            // Configura o tiro do inimigo
            enemy_active->bullet[i].active = true;
            enemy_active->bullet[i].pos_x = enemy_active->pos_x;
            enemy_active->bullet[i].pos_y = enemy_active->pos_y + (al_get_bitmap_height(enemy_active->sprite) / 2); // Centralizado
            enemy_active->bullet[i].speed = 5.0; // Velocidade fixa, ajuste conforme necessário
            break; // Atira apenas uma bala por vez
        }
    }
}

void draw_bullets_enemy(enemy* enemy_active) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (enemy_active->bullet[i].active) {
            float bullet_start_x = enemy_active->bullet[i].pos_x;
            float bullet_start_y = enemy_active->bullet[i].pos_y;
            al_draw_filled_rectangle(bullet_start_x, bullet_start_y - 2, 
                                     bullet_start_x + 10, bullet_start_y + 3, 
                                     al_map_rgb(255, 0, 0));
        }
    }
}

void update_bullets_enemy(bullets* bullet) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullet[i].active) {
            bullet[i].pos_x -= bullet[i].speed; // Move o tiro para a esquerda
            if (bullet[i].pos_x < 0) {
                bullet[i].active = false; // Desativa se sair da tela
            }
        }
    }
}
