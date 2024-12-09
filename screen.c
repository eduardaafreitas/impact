#include "screen.h"

void animate_background(ALLEGRO_BITMAP* background, int* background_x, int speed) {
    *background_x -= speed;
    if (*background_x <= -al_get_bitmap_width(background)) {
        *background_x = 0;  // Reinicia o carrossel
    }

    al_draw_bitmap(background, *background_x, 0, 0);
    al_draw_bitmap(background, *background_x + al_get_bitmap_width(background), 0, 0);
}

void initial_animation(){
    
}

void draw_hud(player_ship *player, ALLEGRO_FONT *font) {
    char hp_text[20];
    char score_text[20];

    sprintf(hp_text, "HP: %d", player->health_points);
    sprintf(score_text, "Score: %d", player->score);

    al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, hp_text);
    al_draw_text(font, al_map_rgb(255, 255, 255), 10, 40, 0, score_text);
}
