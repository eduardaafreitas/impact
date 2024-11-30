#include "sprites.h"
#include "screen.h"
#include "game.h"
#include "enemy.h"

enemy *init_enemy(ALLEGRO_BITMAP* sheet){

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

    sprites_enemy(sheet, enemy_active);
    enemy_active->bullet = alloc_bullets(MAX_BULLETS);

    return enemy_active;
}

void spawn_enemy(enemy *enemy_active){
    // Define posição inicial (fora da tela)
    enemy_active->pos_x = SIZE_X; // Começa na borda direita
    enemy_active->pos_y = rand() % (SIZE_Y - al_get_bitmap_height(enemy_active->sprite)); // Altura aleatória

    // Define outros atributos se necessário
    enemy_active->speed = 3.0 + (rand() % 3); // Velocidade aleatória entre 3.0 e 5.0
    enemy_active->health_points = 3;         // Pontos de vida do inimigo

}

void update_enemy(enemy* enemy_active) {
    // Move o inimigo da direita para a esquerda
    enemy_active->pos_x -= enemy_active->speed;

    // Se o inimigo sair da tela, reposicione ou desative
    if (enemy_active->pos_x + al_get_bitmap_width(enemy_active->sprite) < 0) {
        // Exemplo: reposicionar o inimigo (spawn novamente)
        spawn_enemy(enemy_active);
    }
}


void draw_enemy(enemy *enemy_active){
    al_draw_bitmap(enemy_active->sprite, enemy_active->pos_x, enemy_active->pos_y, 0);
}
