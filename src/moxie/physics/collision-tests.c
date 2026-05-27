#include "moxie/physics.h"
#include "moxie/render.h"
#include "moxie/engine.h"
#include "moxie/transform.h"
#include <stdio.h>

#define fmaxf3(x,y,z) fmaxf(fmaxf(x, y), z)
#define fminf3(x,y,z) fminf(fminf(x, y), z)

static Vec3 AABBOverlap(AABB a, AABB b) {
    if (
        a.max.x < b.min.x || a.min.x > b.max.x ||
        a.max.y < b.min.y || a.min.y > b.max.y ||
        a.max.z < b.min.z || a.min.z > b.max.z
    ) {
        return Vec3Zero;
    }
    
    Vec3 overlap_bmax = Vec3Sub(b.max, a.min);
    Vec3 overlap_amax = Vec3Sub(a.max, b.min);

    Vec3 overlap_min = Vec3Min(overlap_bmax, overlap_amax);
    float smallest_dimension = fminf3(overlap_min.x, overlap_min.y, overlap_min.z);

    Vec3 axis =
        smallest_dimension == overlap_amax.x ? Vec3Right :
        smallest_dimension == overlap_bmax.x ? Vec3Left :
        smallest_dimension == overlap_amax.y ? Vec3Up :
        smallest_dimension == overlap_bmax.y ? Vec3Down :
        smallest_dimension == overlap_amax.z ? Vec3Backward :
        Vec3Forward;

    return Vec3MulF(axis, smallest_dimension);
}
CollisionManifold SweptAABBTest(EntityID first, EntityID second) {
    double dt = GetDeltaTime();

    CollisionManifold null_result = {
        .normal = Vec3Zero,
        .time = -INFINITY,
        .penetration = INFINITY,
        .a = first,
        .b = second,
        .collided = false
    };

    Collision *collision_a = GetCollision(first);
    Transform *transform_a = GetTransform(first);
    RigidBody *rigidbody_a = GetRigidBody(first);

    Collision *collision_b = GetCollision(second);
    Transform *transform_b = GetTransform(second);
    RigidBody *rigidbody_b = GetRigidBody(second);
    
    if (!collision_a || !transform_a || !collision_b || !transform_b) {
        return null_result;
    }
    
    Vec3 velocity_a = rigidbody_a
        ? rigidbody_a->Velocity
        : Vec3Zero;
    
    Vec3 velocity_b = rigidbody_b
        ? rigidbody_b->Velocity
        : Vec3Zero;

    Vec3 velocity = Vec3Sub(velocity_a, velocity_b);
    Vec3 delta_movement = Vec3MulF(velocity, dt);

    AABB aabb_a = TransformAABB(collision_a->aabb, *transform_a);
    AABB aabb_b = TransformAABB(collision_b->aabb, *transform_b);

    CollisionManifold result = null_result;


    Vec3 overlap = AABBOverlap(aabb_a, aabb_b);
    if (Vec3Mag(overlap) > 0.0f) {
        return (CollisionManifold){
            .a = first,
            .b = second,
            .normal = Vec3Inv(Vec3Norm(overlap)),
            .penetration = Vec3Mag(overlap),
            .time = 0.0f,
            .collided = true
        };
    }

    float entry_dist_x = velocity.x >= 0.0f
        ? aabb_b.min.x - aabb_a.max.x
        : aabb_b.max.x - aabb_a.min.x;
    float entry_dist_y = velocity.y >= 0.0f
        ? aabb_b.min.y - aabb_a.max.y
        : aabb_b.max.y - aabb_a.min.y;
    float entry_dist_z = velocity.z >= 0.0f
        ? aabb_b.min.z - aabb_a.max.z
        : aabb_b.max.z - aabb_a.min.z;

    
    float exit_dist_x = velocity.x >= 0.0f
        ? aabb_b.max.x - aabb_a.min.x
        : aabb_b.min.x - aabb_a.max.x;
    float exit_dist_y = velocity.y >= 0.0f
        ? aabb_b.max.y - aabb_a.min.y
        : aabb_b.min.y - aabb_a.max.y;
    float exit_dist_z = velocity.z >= 0.0f
        ? aabb_b.max.z - aabb_a.min.z :
        aabb_b.min.z - aabb_a.max.z;


    float entry_time_x = velocity.x != 0.0f
        ? entry_dist_x / delta_movement.x 
        : -INFINITY;
    float exit_time_x = velocity.x != 0.0f
        ? exit_dist_x / delta_movement.x
        : INFINITY;

    float entry_time_y = velocity.y != 0.0f
        ? entry_dist_y / delta_movement.y
        : -INFINITY;
    float exit_time_y = velocity.y != 0.0f
        ? exit_dist_y / delta_movement.y
        : INFINITY;
    
    float entry_time_z = velocity.z != 0.0f
        ? entry_dist_z / delta_movement.z
        : -INFINITY;
    float exit_time_z = velocity.z != 0.0f
        ? exit_dist_z / delta_movement.z
        : INFINITY;

    float entry_time_max = fmaxf3(entry_time_x, entry_time_y, entry_time_z);
    float exit_time_min = fminf3(exit_time_x, exit_time_y, exit_time_z);

    if (entry_time_max < 0.0f || entry_time_max > 1.0f || entry_time_max > exit_time_min) {
        return null_result;
    }

    Vec3 normal = Vec3Zero;
    if (entry_time_max == entry_time_x) {
        if (
            aabb_a.min.y > aabb_b.max.y || aabb_a.max.y < aabb_b.min.y ||
            aabb_a.min.z > aabb_b.max.z || aabb_a.max.z < aabb_b.min.z
        ) {
            return null_result;
        }
        normal.x = -copysignf(1.0f, velocity.x);
    } else if (entry_time_max == entry_time_y) {
        if (
            aabb_a.min.x > aabb_b.max.x || aabb_a.max.x < aabb_b.min.x ||
            aabb_a.min.z > aabb_b.max.z || aabb_a.max.z < aabb_b.min.z
        ) {
            return null_result;
        }
        normal.y = -copysignf(1.0f, velocity.y);
    } else if (entry_time_max == entry_time_z) {
        if (
            aabb_a.min.x > aabb_b.max.x || aabb_a.max.x < aabb_b.min.x ||
            aabb_a.min.y > aabb_b.max.y || aabb_a.max.y < aabb_b.min.y
        ) {
            return null_result;
        }
        normal.z = -copysignf(1.0f, velocity.z);
    }

    result.collided = true;
    result.normal = normal;
    result.time = entry_time_max;
    
    return result;
}
static void get_magnitude_bounds(
    Collision *collision, Transform transform,
    Vec3 axis,
    float *min, float *max
) {
    *min = INFINITY, *max = -INFINITY;
    
    for (size_t i = 0; i < collision->VertexCount; i ++) {
        Vec3 local_pos =  collision->VertexPositions[i];
        Vec3 world_pos = TransformPoint(local_pos, transform);
        float proj = Vec3Dot(axis, world_pos);
        *min = fminf(*min, proj);
        *max = fmaxf(*max, proj);
    }
}
static bool sat_axis_test(
    Vec3 axis,
    Collision *collision_a, Transform transform_a,
    Collision *collision_b, Transform transform_b,
    Vec3 delta_pos_vector,
    float *min_penetration, Vec3 *penetration_axis,
    float *max_entry_time, float *min_exit_time, Vec3 *entry_axis,
    bool *overlapping
) {
    Vec3 centre_a = TransformPoint(Vec3Zero, transform_a);
    Vec3 centre_b = TransformPoint(Vec3Zero, transform_b);

    if (Vec3Dot(Vec3Sub(centre_a, centre_b), axis) < 0.0f) {
        axis = Vec3Inv(axis);
    }

    float min_a, min_b;
    float max_a, max_b;
    
    get_magnitude_bounds(collision_a, transform_a, axis, &min_a, &max_a);
    get_magnitude_bounds(collision_b, transform_b, axis, &min_b, &max_b);

    bool axis_overlap = (min_a < max_b) && (min_b < max_a);
    if (axis_overlap) {
        float overlap_dist = fminf(max_a, max_b) - fmaxf(min_a, min_b);
        float penetration = fabsf(overlap_dist);
        
        if (penetration < *min_penetration) {
            *min_penetration = penetration;
            *penetration_axis = Vec3Inv(axis);
        }
        return true;
    } else {
        *overlapping = false;
    }

    float delta_pos = Vec3Dot(axis, delta_pos_vector);

    float entry_dist = delta_pos >= 0.0f
        ? min_b - max_a
        : max_b - min_a;
    
    float exit_dist = delta_pos >= 0.0f
        ? max_b - min_a
        : min_b - max_a;

    float entry_time = delta_pos != 0.0f
        ? entry_dist / delta_pos
        : -INFINITY;

    float exit_time = delta_pos != 0.0f
        ? exit_dist / delta_pos
        : INFINITY;
    
    if (
        entry_time < 0.0f ||
        entry_time > 1.0f ||
        entry_time > exit_time
    ) {
        return false;
    }

    if (entry_time > *max_entry_time) {
        *max_entry_time = entry_time;
        *entry_axis = axis;
    }
    if (exit_time < *min_exit_time) {
        *min_exit_time = exit_time;
    }

    return true;
}
static bool FindSeparatingAxis(
    Collision *collision_a, Transform transform_a,
    Collision *collision_b, Transform transform_b,
    Vec3 delta_pos_vector,
    CollisionManifold *result
) {
    Mat4x4 normal_matrix = Mat4Transpose(
        Mat4Inv(Mat4FromTransformNoScale(transform_a))
    );

    bool overlapping = true;
    float min_penetration = INFINITY;
    Vec3 penetration_axis = Vec3Zero;

    float max_entry_time = -INFINITY;
    float min_exit_time = INFINITY;
    Vec3 entry_axis = Vec3Zero;

    for (size_t i = 0; i < collision_a->NormalCount; i ++) {
        Vec3 normal = collision_a->FaceNormals[i];
        Vec4 local_normal = Vec4FromV3(normal);
        Vec3 axis = Vec3Norm(
            ToVec3(Vec4MulM4(local_normal, normal_matrix))
        );

        bool colliding = sat_axis_test(
            axis,
            collision_a, transform_a,
            collision_b, transform_b,
            delta_pos_vector,
            &min_penetration, &penetration_axis,
            &max_entry_time, &min_exit_time, &entry_axis,
            &overlapping
        );
        if (!colliding) { return true; }
    }

    for (size_t i = 0; i < collision_a->EdgeCount; i ++) {
        for (size_t j = 0; j < collision_b->EdgeCount; j ++) {
            Vec3 edge = Vec3Cross(collision_a->Edges[i], collision_b->Edges[j]);
            if (Vec3Mag(edge) < 0.0001f) { continue; }

            Vec4 local_normal = Vec4FromV3(edge);
            Vec3 axis = Vec3Norm(
                ToVec3(Vec4MulM4(local_normal, normal_matrix))
            );

            bool colliding = sat_axis_test(
                axis,
                collision_a, transform_a,
                collision_b, transform_b,
                delta_pos_vector,
                &min_penetration, &penetration_axis,
                &max_entry_time, &min_exit_time, &entry_axis,
                &overlapping
            );
            if (!colliding) { return true; }
        }
    }

    if (min_exit_time < max_entry_time) {
        return true;
    }
    
    if (overlapping) {
        if (min_penetration < result->penetration) {
            result->penetration = min_penetration;
            result->normal = penetration_axis;
            result->time = 0.0f;
        }
    } else if (result->penetration == INFINITY) {
        if (max_entry_time > result->time) {
            result->time = max_entry_time;
            result->normal = entry_axis;
            result->penetration = INFINITY;
        }
    }
    
    result->collided = true;
    return false;
}
CollisionManifold SweptSATTest(EntityID first, EntityID second) {
    Collision *collision_a = GetCollision(first);
    Transform *transform_a = GetTransform(first);
    RigidBody *rigidbody_a = GetRigidBody(first);

    Collision *collision_b = GetCollision(second);
    Transform *transform_b = GetTransform(second);
    RigidBody *rigidbody_b = GetRigidBody(second);

    CollisionManifold null_result = {
        .time = -INFINITY,
        .penetration = INFINITY,
        .collided = false
    };

    if (!collision_a || !transform_a || !collision_b || !transform_b) {
        return null_result;
    }

    Vec3 velocity_a = rigidbody_a
        ? rigidbody_a->Velocity
        : Vec3Zero;

    Vec3 velocity_b = rigidbody_b
        ? rigidbody_b->Velocity
        : Vec3Zero;

    Vec3 relative_velo = Vec3Sub(velocity_a, velocity_b);
    Vec3 delta_pos = Vec3MulF(relative_velo, GetDeltaTime());

    CollisionManifold result_a = null_result;
    result_a.a = first, result_a.b = second;
    
    CollisionManifold result_b = null_result;
    result_b.a = second, result_b.b = first;
        
    if (FindSeparatingAxis(
        collision_a, *transform_a,
        collision_b, *transform_b,
        delta_pos, &result_a
    )) {
        return null_result;
    }
    if (FindSeparatingAxis(
        collision_b, *transform_b,
        collision_a, *transform_a, 
        Vec3Inv(delta_pos), &result_b
    )) {
        return null_result;
    }


    CollisionManifold result;
    if (!rigidbody_b || rigidbody_b->Static) {
        result = result_b.collided ? result_b : result_a;
    } else if (!rigidbody_a || rigidbody_a->Static) {
        result = result_a.collided ? result_a : result_b;
    } else {
        float dot_a = Vec3Dot(result_a.normal, relative_velo);
        float dot_b = Vec3Dot(result_b.normal, relative_velo);

        result = fabsf(dot_b) > fabsf(dot_a) ? result_b : result_a;
    }
    return result;
}