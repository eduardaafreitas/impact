#ifndef __BULLET__
#define __BULLET__
																						
#include "headers.h"

#define MAX_BULLETS 10
#define BULLET_SPEED 5

struct bullets{
    float pos_x, pos_y;
    float speed;
    float speedx;
    float speedy;
    bool active;
};
typedef struct bullets bullets;

bullets *alloc_bullets(int max_bullets);


#endif