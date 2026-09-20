#ifndef PHYSICS_SYSTEM
#define PHYSICS_SYSTEM

#include "./types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool obb_vs_obb(obb_collider *a, obb_collider *b, transform *t0, transform *t1);

#ifdef __cplusplus
}
#endif

#endif