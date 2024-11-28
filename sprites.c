#include "headers.h"
#include "game.h"
#include "screen.h"
#include "player.h"
#include "sprites.h"

void sprites_player (ALLEGRO_BITMAP *sheet, player_ship *player){
    for (int i = 0; i < 5; i++) {
        player->sprites_player[i] = al_create_sub_bitmap(sheet, i * 100, 0, 100, 100);
        if (player->sprites_player[i] == NULL) {
            printf("Erro ao criar sub_bitmap para o sprite %d\n", i);
            // Liberar bitmaps alocados anteriormente em caso de erro
            for (int j = 0; j < i; j++) {
                al_destroy_bitmap(player->sprites_player[j]);
            }
            exit(1);
        }

    }
}

