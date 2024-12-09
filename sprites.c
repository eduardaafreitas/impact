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

void sprites_enemy(ALLEGRO_BITMAP *sheet, enemy *enemy_active, int type) {
    switch (type) {
        case 1:
            enemy_active->sprite = al_create_sub_bitmap(sheet, 0, 0, 80, 80);
            break;
        case 2:
            enemy_active->sprite = al_create_sub_bitmap(sheet, 0, 0, 103, 70);
            break;
        case 3:
            enemy_active->sprite = al_create_sub_bitmap(sheet, 0, 0, 80, 70);
            break;
        case 4:
            enemy_active->sprite = al_create_sub_bitmap(sheet, 0, 0, 95, 70);
            break;
    }

    if(enemy_active->sprite == NULL){
        printf("Erro ao criar sub_bitmap para o sprite enemy\n");
        exit(1);
    }
}


