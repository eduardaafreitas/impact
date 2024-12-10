#include "screen.h"

void animate_background(ALLEGRO_BITMAP* background, int* background_x, int speed) {
    *background_x -= speed;
    if (*background_x <= -al_get_bitmap_width(background)) {
        *background_x = 0;  // Reinicia o carrossel
    }

    al_draw_bitmap(background, *background_x, 0, 0);
    al_draw_bitmap(background, *background_x + al_get_bitmap_width(background), 0, 0);
}

void initial_animation(ALLEGRO_FONT *font_text, ALLEGRO_FONT *font_title){
    al_draw_text(font_title, al_map_rgb(255, 255, 255), SIZE_X / 2, SIZE_Y / 2 - 30,
                 ALLEGRO_ALIGN_CENTER, "Space Wars");
    al_draw_text(font_text, al_map_rgb(255, 255, 255), SIZE_X / 2, SIZE_Y / 2 + 10,
                 ALLEGRO_ALIGN_CENTER, "Pressione ENTER para continuar");
    al_draw_text(font_text, al_map_rgb(255, 255, 255), SIZE_X / 2, SIZE_Y / 2 + 40,
                 ALLEGRO_ALIGN_CENTER, "Pressione TAB para a história");
    al_draw_text(font_text, al_map_rgb(255, 255, 255), SIZE_X / 2, SIZE_Y / 2 + 70,
                 ALLEGRO_ALIGN_CENTER, "Pressione ESC para sair");
    al_flip_display();
}

void initial_text(ALLEGRO_FONT *font_text){
    // Alinhar texto na parte superior esquerda
    al_draw_text(font_text, al_map_rgb(255, 255, 255), 40, 330, ALLEGRO_ALIGN_LEFT,
                 "A Millennium Falcon está sendo perseguida por naves imperiais.");
    al_draw_text(font_text, al_map_rgb(255, 255, 255), 40, 360, ALLEGRO_ALIGN_LEFT,
                 "Você, como piloto da Falcon, deve proteger a nave e alcançar a liberdade.");
    al_draw_text(font_text, al_map_rgb(255, 255, 255), 40, 390, ALLEGRO_ALIGN_LEFT,
                 "Prepare-se para enfrentar ondas de inimigos e chefes perigosos.");
    al_draw_text(font_text, al_map_rgb(255, 255, 255), 40, 420, ALLEGRO_ALIGN_LEFT,
                 "Use todas as suas habilidades para sobreviver e vencer o Império!");
    al_draw_text(font_text, al_map_rgb(255, 255, 255), 40, 450,
                 ALLEGRO_ALIGN_LEFT, "Pressione ENTER para continuar");
}

void draw_hud(player_ship *player, ALLEGRO_FONT *font) {
    char hp_text[20];
    char score_text[20];

    sprintf(hp_text, "HP: %d", player->health_points);
    sprintf(score_text, "Score: %d", player->score);

    al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, hp_text);
    al_draw_text(font, al_map_rgb(255, 255, 255), 10, 40, 0, score_text);
}