#include "collision.h"

unsigned char collision_detect(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2){
    if(x1 > x2 + w2 || x1 + w1 < x2 || y1 > y2 + h2 || y1 + h1 < y2)
        return 0;
    return 1;
}

