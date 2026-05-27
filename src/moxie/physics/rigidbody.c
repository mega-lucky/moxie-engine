#include "moxie/engine.h"
#include "moxie/transform.h"
#include "moxie/physics.h"

static ComponentMask rigid_body_mask = 0;
extern EntityID character, orb;

void InitRigidBodies(void *data) {
    RigidBodySystemState *state = data;

    state->entities = QueryEntities(TransformMask() | rigid_body_mask);
}

static void handle_collision(
    EntityID entity,
    RigidBody *rigidbody, Transform *transform,
    CollisionManifold collision
) {
    if (!collision.collided) { return; }

    Vec3 velocity = rigidbody->Velocity;
    Vec3 normal = entity == collision.b
        ? Vec3Inv(collision.normal)
        : collision.normal;

    float normal_vel_mag = Vec3Dot(velocity, normal);
    Vec3 normal_vel = Vec3MulF(normal, normal_vel_mag);
    Vec3 tangent_vel = Vec3Sub(velocity, normal_vel);
    normal_vel = Vec3MulF(normal_vel, collision.time);
    rigidbody->Velocity = Vec3Add(normal_vel, tangent_vel);

    if (collision.penetration < INFINITY) {
        transform->Position = Vec3Add(
            transform->Position,
            Vec3MulF(collision.normal, collision.penetration)
        );
    }
}

void UpdateRigidBodies(void *data) {
    RigidBodySystemState *state = data;
    double dt = GetDeltaTime();

    EntityList entities = state->entities->Entities;
    for (size_t i = 0; i < EntityList_len(entities); i ++) {
        EntityID entity = EntityList_get(entities, i);
        RigidBody *rigidbody = GetRigidBody(entity);
        Transform *transform = GetTransform(entity);

        if (!rigidbody || rigidbody->Static || !transform) { continue; }

        Vec3 delta_velocity = Vec3MulF(rigidbody->Acceleration, dt * 0.5f);
        rigidbody->Velocity = Vec3Add(rigidbody->Velocity, delta_velocity);

        size_t i = 0;
        const CollisionManifold *ptr = GetCollisionManifold(entity, i);
        while (ptr != NULL) {
            handle_collision(entity, rigidbody, transform, *ptr);
            ptr = GetCollisionManifold(entity, ++i);
        }
        
        transform->Position = Vec3Add(
            transform->Position,
            Vec3MulF(rigidbody->Velocity, dt)
        );

        rigidbody->Velocity = Vec3Add(rigidbody->Velocity, delta_velocity);
    }
}

void RegisterRigidBodyComponent() {
    if (rigid_body_mask != 0) { return; }
    rigid_body_mask = RegisterComponent((ComponentDescription){
        .DataSize = sizeof(RigidBody),
        .Init = NULL,
        .Del = NULL
    });
}
ComponentMask RigidBodyMask() {
    return rigid_body_mask;
}
RigidBody *GetRigidBody(EntityID entity) {
    return GetComponent(entity, rigid_body_mask);
}