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
    enemy_active->type = 0;

    sprites_enemy(sheet, enemy_active, type);
    enemy_active->bullet = alloc_bullets(MAX_BULLETS);

    return enemy_active;
}

void spawn_enemy(enemy *enemy_active) {
    // Define o tipo de inimigo com base no nível atual
    if (wave_level < 5) {
        enemy_active->type = 1; // Apenas voa
    } else if (wave_level < 10) {
        enemy_active->type = 2; // Atira sem mira
        enemy_active->bullet->active = true;
        shoot_enemy(enemy_active);
    } else if (wave_level < 15) {
        enemy_active->type = 3; // Atira mirando
    } else if (wave_level < 20) {
        enemy_active->type = 4; // Atira mais vezes
    } else {
        enemy_active->type = 5; // Boss 1
    }

    // Define posição inicial e configurações gerais
    enemy_active->pos_x = SIZE_X;
    enemy_active->pos_y = rand() % (SIZE_Y - al_get_bitmap_height(enemy_active->sprite));
    enemy_active->speed = 2.0 + (wave_level * 0.1); // Aumenta a velocidade com o nível
    enemy_active->health_points = 3 + wave_level / 5; // Aumenta os pontos de vida com o nível
}

void update_enemy(enemy* enemy_active) {
    // Move o inimigo
    enemy_active->pos_x -= enemy_active->speed;

    // Verifica se o inimigo deve atirar
    static float shoot_timer = 0;
    shoot_timer += 1.0 / 60.0; // Incrementa a cada frame (ajuste para o tempo real do jogo)

    if (shoot_timer >= 1.0) { // Tiros a cada 1 segundo, ajuste conforme necessário
        shoot_enemy(enemy_active);
        shoot_timer = 0; // Reseta o temporizador
    }

    // Se o inimigo sair da tela, reposicionar
    if (enemy_active->pos_x + al_get_bitmap_width(enemy_active->sprite) < 0) {
        spawn_enemy(enemy_active);
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

void draw_bullets_enemy(enemy* enemy_active, bullets *bullet) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (enemy_active->bullet[i].active) {
            // Atualiza a posição do tiro
            enemy_active->bullet[i].pos_y += enemy_active->bullet[i].speedy;

            // Desenha o tiro
            al_draw_filled_rectangle(
                enemy_active->bullet[i].pos_x, enemy_active->bullet[i].pos_y,
                enemy_active->bullet[i].pos_x + 5, enemy_active->bullet[i].pos_y + 10,
                al_map_rgb(255, 0, 0)
            );

            // Desativa o tiro se sair da tela
            if (enemy_active->bullet[i].pos_y > SIZE_Y) {
                enemy_active->bullet[i].active = false;
            }
        }
    }
}

void update_bullets_enemy(bullets *bullet) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullet[i].active) {
            bullet[i].pos_x -= bullet[i].speed; // Move o tiro para a esquerda
            if (bullet[i].pos_x < 0) {
                bullet[i].active = false; // Desativa se sair da tela
            }
        }
    }
}
