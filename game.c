#include "game.h"
#include "enemy.h"

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT event;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* display;
ALLEGRO_FONT* font_title;
ALLEGRO_FONT* font_text;
ALLEGRO_FONT* font_text2;
ALLEGRO_BITMAP* background;
ALLEGRO_BITMAP* background2;
ALLEGRO_BITMAP* background3;
ALLEGRO_BITMAP* sheet_player;
ALLEGRO_BITMAP* sheet_enemy1;
ALLEGRO_BITMAP* sheet_enemy2;
ALLEGRO_BITMAP* sheet_enemy3;
ALLEGRO_BITMAP* sheet_enemy4;
ALLEGRO_BITMAP* sheet_boss1;

int wave_level = 1;          // Começa no nível 1
float elapsed_time = 0.0;    // Tempo decorrido na fase

unsigned char key[ALLEGRO_KEY_MAX];
player_ship *player;
enemy *enemy1;
enemy *enemy2;
enemy *enemy3;
enemy *enemy4;
enemy *boss1;
int background_x = 0;  // Posição horizontal do fundo
const int BACKGROUND_SPEED = 2;  // Velocidade do fundo
bool restart = false;
//-------------------------------------------------------------
void inicia_allegro(bool teste, char *descricao){
    if(teste) 
        return;
    fprintf(stderr, "Não foi possivel inicializar %s\n", descricao);
    exit(1);
}

void inicializando(){
    inicia_allegro(al_init(), "allegro");
    inicia_allegro(al_install_keyboard(), "keyboard");
    inicia_allegro(al_init_image_addon(), "image");
    inicia_allegro(al_init_font_addon(), "font");
    inicia_allegro(al_init_ttf_addon(), "ttf");

    //BASICS:
    timer = al_create_timer(1.0 / 60.0);
    inicia_allegro(timer, "timer");
    queue = al_create_event_queue();
    inicia_allegro(queue, "queue");
    display = al_create_display(SIZE_X, SIZE_Y);
    inicia_allegro(display, "display");

    //FONTES:
    font_title = al_load_font(FONT_TITLE_PATH, 25, 0);
    inicia_allegro(font_title, "fontTitle");
    font_text = al_load_font(FONT_TEXT_PATH, 25, 0);
    inicia_allegro(font_text, "fontTitle");
    font_text2 = al_load_font(FONT_TEXT2_PATH, 25, 0);
    inicia_allegro(font_text2, "fontTitle2");

    //BACKGROUNDS:
    background = al_load_bitmap(PATH_BG1);
    inicia_allegro(background, "background");
    background2 = al_load_bitmap(PATH_BG2);
    inicia_allegro(background2, "background2");
    background3 = al_load_bitmap(PATH_BG3);
    inicia_allegro(background3, "background3");

    //SPRITES PLAYER AND ENEMIES:
    inicia_allegro(al_init_image_addon(), "image addon");
    sheet_player = al_load_bitmap(PATH_PLAYER);

    inicia_allegro(sheet_player, "spritesheetPlayer"); 
    sheet_enemy1 = al_load_bitmap(ENEMY1_PATH);
    inicia_allegro(sheet_enemy1, "spritesheetEnemy1");
    sheet_enemy2 = al_load_bitmap(ENEMY2_PATH);
    inicia_allegro(sheet_enemy2, "spritesheetEnemy2");
    sheet_enemy3 = al_load_bitmap(ENEMY3_PATH);
    inicia_allegro(sheet_enemy3, "spritesheetEnemy3");
    sheet_enemy4 = al_load_bitmap(ENEMY4_PATH);
    inicia_allegro(sheet_enemy4, "spritesheetEnemy4");
    sheet_boss1 = al_load_bitmap(BOSS1_PATH);
    inicia_allegro(sheet_boss1, "spritesheetboss1");
  
    player = init_player(sheet_player);
    enemy1 = init_enemy(sheet_enemy1, 1);
    enemy2 = init_enemy(sheet_enemy2, 2);
    enemy3 = init_enemy(sheet_enemy3, 3);
    enemy4 = init_enemy(sheet_enemy4, 4);    
    boss1 = init_enemy(sheet_boss1, 5);

	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_start_timer(timer);
}

void state_start() {
    al_init_primitives_addon();   
    inicializando();

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(background, 0, 0, 0);
    initial_animation(font_text, font_title);
    al_flip_display();
    
    while (state == start) {
        al_wait_for_event(queue, &event);
        

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            state = end_game;  // Encerrar o jogo

        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                state = playing;  // Avançar para o próximo estado

            } else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                state = end_game;  // Encerrar o jogo
            }
        }
    }
}

void state_playing() {
    background_x = 0;
    memset(key, 0, sizeof(key));

    while (state == playing) {
        al_wait_for_event(queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                state = end_game;
                break;

            case ALLEGRO_EVENT_TIMER:
                al_clear_to_color(al_map_rgb(0, 0, 0));
                animate_background(background3, &background_x, BACKGROUND_SPEED);
                
                entry_identifyer(key, player);

                // Atualiza inimigos com base na onda
                if (wave_level < 3) {
                    manage_enemy_wave(enemy1, player, font_text);
                } else if (wave_level < 5) {
                    manage_enemy_wave(enemy2, player, font_text);
                } else if (wave_level < 7) {
                    manage_enemy_wave(enemy3, player, font_text);
                } else if (wave_level > 9) {
                    manage_enemy_wave(enemy4, player, font_text);
                }

                al_flip_display();
                // Atualiza a onda
                update_wave_level(player);
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = 1;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] = 0;
                break;
        }
    }
}

void state_fase2() {
    background_x = 0;
    memset(key, 0, sizeof(key));

    while (state == fase2) {
        al_wait_for_event(queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                state = end_game;
                break;

            case ALLEGRO_EVENT_TIMER:
                al_clear_to_color(al_map_rgb(0, 0, 0));
                animate_background(background2, &background_x, BACKGROUND_SPEED);

                entry_identifyer(key, player);

                // Aqui, você pode usar inimigos ou lógica diferentes para a Fase 2
                if (wave_level < 13) {
                    manage_enemy_wave(enemy3, player, font_text);
                } else {
                    manage_enemy_wave(enemy4, player, font_text);
                }

                al_flip_display();
                update_wave_level(player); // Continua atualizando o nível
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = 1;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] = 0;
                break;
        }
    }
}

void state_pause() {

    //AJEITAR TELA DE PAUSA
    //al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font_title, al_map_rgb(255, 255, 255), SIZE_X / 2, SIZE_Y / 2 - 30,
                 ALLEGRO_ALIGN_CENTER, "Jogo Pausado");
    al_draw_text(font_text, al_map_rgb(255, 255, 255), SIZE_X / 2, SIZE_Y / 2 + 10,
                 ALLEGRO_ALIGN_CENTER, "Pressione ENTER para continuar");
    al_draw_text(font_text, al_map_rgb(255, 255, 255), SIZE_X / 2, SIZE_Y / 2 + 40,
                 ALLEGRO_ALIGN_CENTER, "Pressione ESC para sair");
    al_flip_display();

    while (state == playing) {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            state = end_game;
            break;
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                return;  // Sai do estado de pausa e volta ao jogo
            } else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                state = end_game;  // Encerrar o jogo
                break;
            }
        }
    }
}

void state_end_game() {
    // Libera todos os recursos alocados
    al_destroy_bitmap(background);
    al_destroy_bitmap(background2);
    al_destroy_bitmap(background3);
    al_destroy_bitmap(sheet_player);
    al_destroy_bitmap(sheet_enemy1);
    al_destroy_bitmap(sheet_enemy2);
    al_destroy_bitmap(sheet_enemy3);
    al_destroy_bitmap(sheet_enemy4);
    al_destroy_font(font_title);
    al_destroy_font(font_text);
    al_destroy_font(font_text2);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    printf("Jogo encerrado com sucesso.\n");
    exit(0);  // Encerra o programa
}

void state_game_over() {
    // Variáveis de controle do loop

    while (state == game_over) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        // Lida com eventos
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            state = end_game;
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                state = end_game;
            } else if (event.keyboard.keycode == ALLEGRO_KEY_R) {
                restart = true;
                state = playing;
            }
        } else if (event.type == ALLEGRO_EVENT_TIMER) {
            // Desenha a tela de Game Over
            //al_clear_to_color(al_color_name("black"));

            al_draw_text(font_title, al_map_rgb(255, 255, 255),
                         al_get_display_width(display) / 2, 
                         al_get_display_height(display) / 3, 
                         ALLEGRO_ALIGN_CENTER, "GAME OVER");
            
            char score_text[50];
            snprintf(score_text, sizeof(score_text), "Your Score: %d", player->score);
            al_draw_text(font_text, al_map_rgb(255, 255, 255),
                         al_get_display_width(display) / 2,
                         al_get_display_height(display) / 2,
                         ALLEGRO_ALIGN_CENTER, score_text);

            al_draw_text(font_text, al_map_rgb(255, 255, 255),
                         al_get_display_width(display) / 2,
                         (al_get_display_height(display) / 2) + 50,
                         ALLEGRO_ALIGN_CENTER, "Press ESC to Exit");
            
            al_draw_text(font_text, al_map_rgb(255, 255, 255),
                         al_get_display_width(display) / 2,
                         (al_get_display_height(display) / 2) + 100,
                         ALLEGRO_ALIGN_CENTER, "Press R to Restart");

            al_flip_display();
        }
    }

    // Limpa recursos
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}

void entry_identifyer(unsigned char *key, player_ship *player){
    if (key[ALLEGRO_KEY_UP]) {
        player->atual_pose = up;  // Muda para sprite "cima"
    } else if (key[ALLEGRO_KEY_DOWN]) {
        player->atual_pose = down; // Muda para sprite "baixo"
    } else if (key[ALLEGRO_KEY_LEFT]) {
        player->atual_pose = back; // Muda para "trás"
    } else if (key[ALLEGRO_KEY_RIGHT]) {
        player->atual_pose = front; // Muda para "frente"
    } else if (key[ALLEGRO_KEY_ESCAPE]){
        state_pause();
    }

}

void update_wave_level(player_ship* player) {
    if (player->enemies_defeated == 2 && player->enemies_defeated > 0) {
        wave_level++;
        player->enemies_defeated = 0; // Reinicia contagem

        if (wave_level > 10) { // Supondo que a mudança ocorre após o nível 10
            state = fase2;
        }
    }
}

void manage_enemy_wave(enemy* enemy_wave, player_ship* player, ALLEGRO_FONT* font) {
    update_enemy(enemy_wave);
    draw_enemy(enemy_wave);
    
    // Verificar se o jogador foi atingido por alguma bala dos inimigos
    if (enemy_wave->bullet->active == true) {
        update_bullets_enemy(enemy_wave->bullet);
        draw_bullets_enemy(enemy_wave);
        if (check_collision(player, enemy_wave->bullet)) {
            player->health_points --;
            enemy_wave->bullet->active = false;  // Desativa a bala após o impacto
        }
    }

    update_player(event, player, enemy_wave);
    draw_player(player);
    draw_hud(player, font);
}