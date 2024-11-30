
																											
#include "game.h"

// Definição da variável global
enum game_state state; 

int main(){

	state = start;

    while (1) {
        switch (state) {
            case start:
                state_start();
                break;
            case playing:
                state_playing();
                break;
            case end_game:
                state_end_game();
                break;
            default:
                printf("Estado desconhecido! Encerrando o jogo...\n");
                state = end_game;
                break;
        }
    }

    return 0;
}