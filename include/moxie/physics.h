#ifndef __PHYSICS_H
#define __PHYSICS_H

#include "moxmath/moxmath.h"
#include "moxie/world.h"
#include "moxie/render.h"

typedef struct Collision {
    AABB aabb;
    Vec3 *FaceNormals;
    Vec3 *VertexPositions;
    Vec3 *Edges;
    size_t NormalCount;
    size_t VertexCount;
    size_t EdgeCount;
    bool NonCollidable;
} Collision;

typedef struct CollisionSystemState {
    EntityQuery *entities; 
} CollisionSystemState;

typedef struct RigidBody {
    Vec3 Velocity;
    Vec3 Acceleration;
    Vec3 AngularVelocity;
    Vec3 AngularAcceleration;
    bool Static;
} RigidBody;

typedef struct RigidBodySystemState {
    EntityQuery *entities;
} RigidBodySystemState;

typedef struct CollisionManifold {
    Vec3 normal;
    float time;
    float penetration;
    EntityID a, b;
    bool collided;
} CollisionManifold;

void CollisionSystemInit(void *data);
void CollisionSystemUpdate(void *data);

void InitRigidBodies(void *data);
void UpdateRigidBodies(void *data);

CollisionManifold SweptAABBTest(EntityID first, EntityID second);
CollisionManifold SweptSATTest(EntityID first, EntityID second);

void InitCollisionFromMesh(Mesh *mesh, Collision *collision);

void RegisterRigidBodyComponent();
ComponentMask RigidBodyMask();
RigidBody *GetRigidBody(EntityID entity);

void RegisterCollisionComponent();
ComponentMask CollisionMask();
Collision *GetCollision(EntityID entity);

const CollisionManifold *GetCollisionManifold(EntityID entity, size_t i);

#endif