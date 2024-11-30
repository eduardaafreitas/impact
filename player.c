																										

#include "player.h"
#include "sprites.h"
#include "screen.h"
#include "game.h"

player_ship *init_player(ALLEGRO_BITMAP* sheet){
    
    player_ship *player;
    player = malloc(sizeof(player_ship));
    if (player == NULL){
        printf("Erro ao alocar memoria (player)\n");
        exit(1);
    }
    player->pos_x = 50;
    player->pos_y = SIZE_Y/2;  
    player->speed = 5.0;
    player->score = 0;
    player->health_points = 10;
    
    sprites_player(sheet, player);
    player->atual_pose = standard;
    player->bullet = alloc_bullets(MAX_BULLETS);

    return player;
}

void update_player(ALLEGRO_EVENT event, player_ship *player) {
    // Movimenta a nave enquanto a tecla está pressionada
    if (key[ALLEGRO_KEY_UP]) {
        player->pos_y -= player->speed;
    }
    if (key[ALLEGRO_KEY_DOWN]) {
        player->pos_y += player->speed;
    }
    if (key[ALLEGRO_KEY_LEFT]) {
        player->pos_x -= player->speed;
    }
    if (key[ALLEGRO_KEY_RIGHT]) {
        player->pos_x += player->speed;
    }

    // Limitar o jogador dentro da tela
    if (player->pos_y < 0) player->pos_y = 0;
    if (player->pos_y > SIZE_Y - al_get_bitmap_height(player->sprites_player[standard])) 
        player->pos_y = SIZE_Y - al_get_bitmap_height(player->sprites_player[standard]);
    if (player->pos_x < 0) player->pos_x = 0;
    if (player->pos_x > SIZE_X - al_get_bitmap_width(player->sprites_player[standard])) 
        player->pos_x = SIZE_X - al_get_bitmap_width(player->sprites_player[standard]);

    // Processa evento de tecla solta para voltar à pose padrão
    if (event.type == ALLEGRO_EVENT_KEY_UP) {
        key[event.keyboard.keycode] = 0;  // Marca a tecla como não pressionada
    }

        // Dispara os tiros
    if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
        shoot(player);
    }

    // Atualiza os tiros
    update_bullets(player);

}

void draw_player(player_ship *player){
    al_draw_bitmap(player->sprites_player[player->atual_pose], player->pos_x, player->pos_y, 0);
    draw_bullets(player);
}

bullets *alloc_bullets(int max_bullets) {
    bullets *bullet = malloc(max_bullets * sizeof(bullets));
    if (bullet == NULL) {
        printf("Erro ao alocar memória para as balas\n");
        exit(1);
    }

    // Inicializa as balas
    for (int i = 0; i < max_bullets; i++) {
        bullet[i].active = false;
        bullet[i].pos_x = 0.0;
        bullet[i].pos_y = 0.0;
        bullet[i].speed = 0.0;
    }

    return bullet;
}
void free_player(player_ship *player) {
    if (player != NULL) {
        if (player->bullet != NULL) {
            free(player->bullet);
            player->bullet = NULL; // Prevenir uso após liberar
        }
        free(player);
        player = NULL;
    }
}
void shoot(player_ship *player){
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!player->bullet[i].active) {  // Procura por um tiro inativo
            player->bullet[i].pos_x = player->pos_x + 20; // Posição inicial
            player->bullet[i].pos_y = player->pos_y + 10; // Ajustado para o centro
            player->bullet[i].speed = 8.0;            // Velocidade do tiro
            player->bullet[i].active = true;          // Marca o tiro como ativo
            break;
        }
    }
}

void update_bullets(player_ship *player) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (player->bullet[i].active) {
            player->bullet[i].pos_x += player->bullet[i].speed;  // Move o tiro para a direita

            // Desativa o tiro se ele sair da tela
            if (player->bullet[i].pos_x > SIZE_X) {
                player->bullet[i].active = false;
            }
        }
    }
}

void draw_bullets(player_ship *player) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (player->bullet[i].active) {
            // Desenha o tiro como um retângulo (pode mudar para círculo ou outro formato)
            al_draw_filled_rectangle(player->bullet[i].pos_x, player->bullet[i].pos_y, 
                                     player->bullet[i].pos_x + 10, player->bullet[i].pos_y + 5, 
                                     al_map_rgb(255, 255, 0));
        }
    }
}

