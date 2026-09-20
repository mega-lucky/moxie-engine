#ifndef PHYSICS_TYPES
#define PHYSICS_TYPES

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include <cglm/cglm.h>

typedef struct sat_test_result {
    vec3 normal;
    float overlap_depth;
    float entry_time;
} sat_test_result;

typedef struct aabb_collider {
    bool scales_with_transform;
    vec3 min, max;
} aabb_collider;

typedef struct obb_collider {
    bool scales_with_transform;
    vec3 min, max;
} obb_collider;

typedef struct custom_collider {
    vec3 *vertices; size_t nverts;
} custom_collider;

typedef struct mesh_shape_collider {
    size_t pos_attrib;
} mesh_shape_collider;

typedef struct shape {
    size_t n_verts;
    vec3 *verts;
} shape;

#endif