#include "screen.h"

void animate_background(ALLEGRO_BITMAP* background, int* background_x, int speed) {
    *background_x -= speed;
    if (*background_x <= -al_get_bitmap_width(background)) {
        *background_x = 0;  // Reinicia o carrossel
    }

    al_draw_bitmap(background, *background_x, 0, 0);
    al_draw_bitmap(background, *background_x + al_get_bitmap_width(background), 0, 0);
}
