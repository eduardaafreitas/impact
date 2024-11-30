#include "bullet.h"

bullets *alloc_bullets(int max_bullets) {
    bullets *bullet = malloc(max_bullets * sizeof(bullets));
    if (bullet == NULL) {
        printf("Erro ao alocar memória para as balas\n");
        exit(1);
    }

    // Inicializa as balas
    for (int i = 0; i < max_bullets; i++) {
        bullet[i].active = false;
        bullet[i].pos_x = 0.0;
        bullet[i].pos_y = 0.0;
        bullet[i].speed = 0.0;
    }

    return bullet;
}