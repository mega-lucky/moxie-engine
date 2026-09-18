#ifndef PHYSICS_TYPES
#define PHYSICS_TYPES

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include <cglm/cglm.h>

typedef struct aabb_collider {
    bool active;
    vec3 min, max;
} aabb_collider;

typedef struct obb_collider {
    bool active;
    vec3 min, max; versor r;
} obb_collider;

typedef struct custom_collider {
    bool active;
    vec3 *vertices; size_t nverts;
} custom_collider;

typedef struct mesh_shape_collider {
    bool active;
    size_t pos_attrib;
} mesh_shape_collider;

#endif