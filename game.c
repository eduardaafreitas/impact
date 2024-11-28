
#include "headers.h"																											
#include "screen.h"
#include "player.h"
#include "game.h"

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT event;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* display;
ALLEGRO_FONT* font;
ALLEGRO_BITMAP* background;
ALLEGRO_BITMAP* background2;
ALLEGRO_BITMAP* sheet_player;

unsigned char key[ALLEGRO_KEY_MAX];
player_ship *player;
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

    timer = al_create_timer(1.0 / 60.0);
    inicia_allegro(timer, "timer");

    queue = al_create_event_queue();
    inicia_allegro(queue, "queue");

    display = al_create_display(SIZE_X, SIZE_Y);
    inicia_allegro(display, "display");

    // font = al_load_font("resources/fonts/fightkid.ttf", 25, 0);
    // inicia_allegro(font, "font");

    background = al_load_bitmap(PATH_BG1);
    inicia_allegro(background, "background");

    background2 = al_load_bitmap(PATH_BG2);
    inicia_allegro(background2, "background2");

    inicia_allegro(al_init_image_addon(), "image addon");
    sheet_player = al_load_bitmap(PATH_PLAYER);
    inicia_allegro(sheet_player, "spritesheetPlayer"); 

    player = init_player(sheet_player);

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

                update_player(event, player);
                draw_player(player);
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
    
    } else if (key[ALLEGRO_KEY_SPACE]){
        shoot(player);
    }

}

// void draw_all(){

// }
