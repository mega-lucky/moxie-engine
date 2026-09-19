#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__
#include <cglm/cglm.h>

typedef struct transform {
    vec3 position;
    vec3 scale;
    versor rotation;
} transform;

#endif