

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
ALLEGRO_BITMAP* sheet_player;
ALLEGRO_BITMAP* sheet_enemy1;

unsigned char key[ALLEGRO_KEY_MAX];
player_ship *player;
enemy *enemy1;
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

    //SPRITES PLAYER AND ENEMIES:
    inicia_allegro(al_init_image_addon(), "image addon");
    sheet_player = al_load_bitmap(PATH_PLAYER);
    inicia_allegro(sheet_player, "spritesheetPlayer"); 
    sheet_enemy1 = al_load_bitmap(ENEMY1_PATH);
    inicia_allegro(sheet_enemy1, "spritesheetEnemy");
    player = init_player(sheet_player);
    enemy1 = init_enemy(sheet_enemy1);

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

        switch (event.type){
            case ALLEGRO_EVENT_DISPLAY_CLOSE: //default
                state = end_game;
                break;

            case ALLEGRO_EVENT_TIMER:
                al_clear_to_color(al_map_rgb(0, 0, 0));
                animate_background(background2, &background_x, BACKGROUND_SPEED);
                entry_identifyer(key, player);
                update_enemy(enemy1);
                update_player(event, player, enemy1);
                draw_player(player);
                draw_enemy(enemy1);
                al_flip_display();
                break;
            
            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = 1;  // Marca a tecla como pressionada
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] = 0;  // Marca a tecla como não pressionada
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

// void draw_all(){

// }
