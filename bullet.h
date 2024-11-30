#ifndef __BULLET__
#define __BULLET__
																						
#include "headers.h"

#define MAX_BULLETS 10

struct bullets{
    float pos_x, pos_y;
    float speed;
    bool active;
};
typedef struct bullets bullets;

bullets *alloc_bullets(int max_bullets);


#endif