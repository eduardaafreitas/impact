#include "player.h"
#include "sprites.h"
#include "screen.h"
#include "game.h"
#include "collision.h"

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
    player->time_since_last_shot = 0.0;
    player->shot_cooldown = 0.25;
    player->enemies_defeated = 0;

    return player;
}

void update_player(ALLEGRO_EVENT event, player_ship *player, enemy *enemy_active) {
    // Atualiza o tempo desde o último disparo
    player->time_since_last_shot += 1.0 / 60.0; // Assumindo 60 FPS

    // Movimenta a nave enquanto a tecla está pressionada
    if (key[ALLEGRO_KEY_UP]) player->pos_y -= player->speed;
    if (key[ALLEGRO_KEY_DOWN]) player->pos_y += player->speed;
    if (key[ALLEGRO_KEY_LEFT]) player->pos_x -= player->speed;
    if (key[ALLEGRO_KEY_RIGHT]) player->pos_x += player->speed;

    // Limitar o jogador dentro da tela
    if (player->pos_y < 0) player->pos_y = 0;
    if (player->pos_y > SIZE_Y - al_get_bitmap_height(player->sprites_player[standard])) 
        player->pos_y = SIZE_Y - al_get_bitmap_height(player->sprites_player[standard]);
    if (player->pos_x < 0) player->pos_x = 0;
    if (player->pos_x > SIZE_X - al_get_bitmap_width(player->sprites_player[standard])) 
        player->pos_x = SIZE_X - al_get_bitmap_width(player->sprites_player[standard]);

    // Dispara tiros contínuos
    if (key[ALLEGRO_KEY_SPACE] && player->time_since_last_shot >= player->shot_cooldown) {
        shoot_player(player);
        player->time_since_last_shot = 0; // Reseta o cooldown após disparar
    }

    // Atualiza os tiros
    update_bullets_player(player, enemy_active);
}


void draw_player(player_ship *player){
    al_draw_bitmap(player->sprites_player[player->atual_pose], player->pos_x, player->pos_y, 0);
    draw_bullets_player(player);
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
void shoot_player(player_ship *player){
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

void update_bullets_player(player_ship *player, enemy *enemy_active) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (player->bullet[i].active) {
            // Move o tiro
            player->bullet[i].pos_x += player->bullet[i].speed;

            // Desativa o tiro se sair da tela
            if (player->bullet[i].pos_x > SIZE_X) {
                player->bullet[i].active = false;
                continue;
            }

            // Verifica colisão com o inimigo
            float bullet_width = 10;  // Largura do tiro
            float bullet_height = 5;  // Altura do tiro
            float enemy_width = al_get_bitmap_width(enemy_active->sprite);
            float enemy_height = al_get_bitmap_height(enemy_active->sprite);

            if (collision_detect(player->bullet[i].pos_x, player->bullet[i].pos_y, bullet_width, bullet_height,
                                enemy_active->pos_x, enemy_active->pos_y, enemy_width, enemy_height)) {
                player->bullet[i].active = false; // Desativa o tiro
                enemy_active->health_points--;   // Diminui a vida do inimigo

                // Se o inimigo for derrotado, reposicione-o ou aplique lógica adicional
                if (enemy_active->health_points <= 0) {
                    spawn_enemy(enemy_active);  // Reposiciona o inimigo
                    player->enemies_defeated++;
                    player->score += 10;        // Incrementa a pontuação do jogador
                }
            }
        }
    }
}


void draw_bullets_player(player_ship *player) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (player->bullet[i].active) {
            // Desenha o tiro como um retângulo (pode mudar para círculo ou outro formato)
            al_draw_filled_rectangle(player->bullet[i].pos_x, player->bullet[i].pos_y, 
                                     player->bullet[i].pos_x + 10, player->bullet[i].pos_y + 5, 
                                     al_map_rgb(255, 255, 0));
        }
    }
}

