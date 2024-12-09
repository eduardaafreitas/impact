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

int wave_level = 1;          // Começa no nível 1
float elapsed_time = 0.0;    // Tempo decorrido na fase

unsigned char key[ALLEGRO_KEY_MAX];
player_ship *player;
enemy *enemy1;
enemy *enemy2;
enemy *enemy3;
enemy *enemy4;
int background_x = 0;  // Posição horizontal do fundo
const int BACKGROUND_SPEED = 2;  // Velocidade do fundo
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
  
    player = init_player(sheet_player);
    enemy1 = init_enemy(sheet_enemy1, 1);
    enemy2 = init_enemy(sheet_enemy2, 2);
    enemy3 = init_enemy(sheet_enemy3, 3);
    enemy4 = init_enemy(sheet_enemy4, 4);    

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
                animate_background(background2, &background_x, BACKGROUND_SPEED);
                
                entry_identifyer(key, player);

                // Atualiza inimigos com base na onda
                if (wave_level < 3) {
                    update_enemy(enemy1);
                    draw_enemy(enemy1);
                    if (check_collision(player, enemy1)) {
                        // Colisão com o inimigo
                        player->health_points -= 10;  // Diminui vida do jogador
                        // Pode adicionar mais lógica de colisão aqui (efeitos, sons, etc)
                    }
                    update_player(event, player, enemy1);
                    draw_player(player);
                    draw_hud(&player, font_text);
                } else if (wave_level < 5) {
                    update_enemy(enemy2);
                    draw_enemy(enemy2);
                    if (check_collision(player, enemy2)) {
                        player->health_points -= 10;
                    }
                    update_bullets_enemy(enemy2->bullet);
                    draw_bullets_enemy(enemy2, enemy2->bullet);
                    update_player(event, player, enemy2);
                    draw_player(player);
                    draw_hud(&player, font_text);
                } else if (wave_level < 7) {
                    update_enemy(enemy3);
                    draw_enemy(enemy3);
                    if (check_collision(player, enemy3)) {
                        player->health_points -= 10;
                    }
                    update_bullets_enemy(enemy3->bullet);
                    draw_bullets_enemy(enemy3, enemy3->bullet);
                    update_player(event, player, enemy3);
                    draw_player(player);
                    draw_hud(&player, font_text);
                } else if (wave_level > 9){
                    update_enemy(enemy4);
                    draw_enemy(enemy4);
                    if (check_collision(player, enemy4)) {
                        player->health_points -= 10;
                    }
                    update_bullets_enemy(enemy4->bullet);
                    draw_bullets_enemy(enemy4, enemy4->bullet);
                    update_player(event, player, enemy4);
                    draw_player(player);
                    draw_hud(&player, font_text);
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


void state_pause() {

    //AJEITAR TELA DE PAUSA
    al_clear_to_color(al_map_rgb(0, 0, 0));
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
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    printf("Jogo encerrado com sucesso.\n");
    exit(0);  // Encerra o programa
}

void update_wave_level(player_ship* player) {
    // Incrementa o nível da onda a cada 30 segundos
    // if ((int)elapsed_time % 30 == 0) {
    //     wave_level++;
    // }

    // Avança a onda após derrotar 10 inimigos
    if (player->enemies_defeated % 5 == 0 && player->enemies_defeated > 0) {
        wave_level++;
        player->enemies_defeated = 0; // Reinicia contagem
    }
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
