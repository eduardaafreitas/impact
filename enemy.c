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
    enemy_active->can_shoot = (type == 2 || type == 3); // Apenas certos tipos podem atirar


    sprites_enemy(sheet, enemy_active, type);
    enemy_active->bullet = alloc_bullets(MAX_BULLETS);

    return enemy_active;
}

void spawn_enemy(enemy *enemy_active) {
    //Define o tipo de inimigo com base no nível atual
    if (wave_level < 3) {
        enemy_active->type = 1;
    }else if (wave_level < 5) {
        enemy_active->type = 2;
        // enemy_active->bullet->active = true;
        // shoot_enemy(enemy_active);
    } else if (wave_level < 7) {
        enemy_active->type = 3;
    } else if (wave_level < 8) {
        enemy_active->type = 4;
    }else {
        enemy_active->type = 5; // Boss 1
    }

    // Define posição inicial e configurações gerais
    enemy_active->pos_x = SIZE_X;
    enemy_active->pos_y = rand() % (SIZE_Y - al_get_bitmap_height(enemy_active->sprite));
    enemy_active->speed = 2.0 + (wave_level * 0.1); // Aumenta a velocidade com o nível
    enemy_active->health_points = 3 + wave_level / 5; // Aumenta os pontos de vida com o nível

    // enemy_active->pos_y = SIZE_Y - al_get_bitmap_height(enemy_active->sprite); // Posição fixa na parte inferior da tela
    // enemy_active->speed = 1.0 + (wave_level * 0.1); // Aumenta a velocidade com o nível
    // enemy_active->health_points = 5 + wave_level / 5; // Aumenta os pontos de vida com o nível
    
}

void spawn_boss(enemy *enemy_active) {
    if (enemy_active->type == 5) {
        enemy_active->pos_x = SIZE_X;  // Posição inicial na parte direita da tela
        enemy_active->pos_y = SIZE_Y - al_get_bitmap_height(enemy_active->sprite);  // Posição fixa na parte inferior da tela
        enemy_active->speed = 0.5 + (wave_level * 0.1);  // Ajusta a velocidade do Boss
        enemy_active->health_points = 10 + wave_level / 5;  // Define os pontos de vida do Boss
    }
}


void update_enemy(enemy* enemy_active) {
    // Move o inimigo

    if (enemy_active == NULL) {
        return; // Se não houver inimigo ativo, não faça nada
    }

    // Verifica se o inimigo é do tipo 5 (Boss)
    if (enemy_active->type == 5) {
        // Chama a função para fazer o spawn do Boss
        spawn_boss(enemy_active);
    }

    if (enemy_active->type != 5) {
        // Lógica para inimigos normais (enemy1, enemy2, etc.)
        enemy_active->pos_x -= enemy_active->speed;  // Exemplo de movimento para a esquerda
        if (enemy_active->can_shoot) {
            enemy_active->shoot_timer += 1.0 / 60.0; // Incrementa o temporizador
            if (enemy_active->shoot_timer >= 1.0) { // Verifica o intervalo de disparo
                shoot_enemy(enemy_active);
                enemy_active->shoot_timer = 0.0; // Reseta o temporizador
            }
        }
    }

    if (enemy_active->pos_x < 0 || enemy_active->health_points <= 0) {
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
