#ifndef PHYSICS_SYSTEM
#define PHYSICS_SYSTEM

#include "./types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool obb_vs_obb_static(obb_collider *a, obb_collider *b, transform *t0, transform *t1);
bool obb_vs_obb_swept(obb_collider *a, obb_collider *b, transform *t0, transform *t1, vec3 delta);

#ifdef __cplusplus
}
#endif

#endif