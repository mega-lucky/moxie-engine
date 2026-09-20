#include "./types.h"
#include <string.h>
#include <transform.h>

static inline void gen_obb_vertices(obb_collider *obb, transform *t, vec3 *out) {
    mat3 r; glm_quat_mat3(t->rotation, r);

    vec3 c, s;
    glm_vec3_add(obb->min, obb->max, c);
    glm_vec3_divs(c, 2.0f, c);
    glm_mat3_mulv(r, c, c);
    glm_vec3_add(c, t->position, c);
    
    glm_vec3_sub(obb->max, obb->min, s);
    glm_vec3_divs(s, 2.0f, s);
    if (obb->scales_with_transform) {
        glm_vec3_mul(s, t->scale, s);
    }

    vec3 result[8] = {
        { s[0],  s[1],  s[2]}, {-s[0],  s[1],  s[2]},
        { s[0], -s[1],  s[2]}, { s[0],  s[1], -s[2]},
        { s[0], -s[1], -s[2]}, {-s[0],  s[1], -s[2]},
        {-s[0], -s[1],  s[2]}, {-s[0], -s[1], -s[2]},
    };

    memcpy(out, result, sizeof(vec3) * 8);

    for (int i = 0; i < 8; i ++) {
        glm_mat3_mulv(r, out[i], out[i]);
        glm_vec3_add(out[i], c, out[i]);
    }
}

static inline void gen_obb_normals(versor q, vec3 *out) {
    glm_quat_rotatev(q, GLM_XUP, out[0]);
    glm_quat_rotatev(q, GLM_YUP, out[1]);
    glm_quat_rotatev(q, GLM_ZUP, out[2]);
}

static inline void bb_cross_axes(vec3 *a_normals, vec3 *b_normals, vec3 *out) {
    for (int i = 0; i < 3; i ++) {
        for (int j = 0; j < 3; j ++) {
            glm_vec3_cross(a_normals[i], b_normals[j], out[i * 3 + j]);
        }
    }
}

static inline void sat_min_max(shape *a, vec3 axis, float *min, float *max) {
    *min = FLT_MAX; *max = -FLT_MAX;

    for (size_t i = 0; i < a->n_verts; i ++) {
        float projection = glm_vec3_dot(axis, a->verts[i]);
        *min = fminf(*min, projection);
        *max = fmaxf(*max, projection);
    }
}

static inline bool seperating_axis_static(shape *a, shape *b, vec3 *axes, size_t n_axes, sat_test_result *out) {
    sat_test_result result;
    result.overlap_depth = FLT_MAX;
    
    for (size_t i = 0; i < n_axes; i ++) {
        if (glm_vec3_norm2(axes[i]) < GLM_FLT_EPSILON) {
            continue;
        }

        float *axis = axes[i];
        float min0, max0, min1, max1;

        sat_min_max(a, axis, &min0, &max0);
        sat_min_max(b, axis, &min1, &max1);

        if (max0 < min1 || max1 < min0) {
            return true;
        }

        float depth = fminf(
            fabsf(max0 - min1),
            fabsf(min0 - max1)
        );

        if (out && result.overlap_depth > depth) {
            result.overlap_depth = depth;
            glm_vec3_copy(axis, result.normal);
        }
    }


    result.entry_time = 0.0f;
    memcpy(out, &result, sizeof(result));
    return false;
}

static inline bool seperating_axis_swept(shape *a, shape *b, vec3 delta, vec3 *axes, size_t n_axes, sat_test_result *out) {
    if (glm_vec3_norm2(delta) < GLM_FLT_EPSILON) {
        return seperating_axis_static(a, b, axes, n_axes, out);
    }

    sat_test_result result;
    result.overlap_depth = FLT_MAX;
    result.entry_time = FLT_MAX;
    
    for (size_t i = 0; i < n_axes; i ++) {
        if (glm_vec3_norm2(axes[i]) < GLM_FLT_EPSILON) {
            continue;
        }

        float *axis = axes[i];
        float min0, max0, min1, max1;

        sat_min_max(a, axis, &min0, &max0);
        sat_min_max(b, axis, &min1, &max1);

        if (max0 >= min1 && max1 >= min0) {
            if (result.entry_time > 0.0f && result.entry_time <= 1.0f) {
                continue;
            }

            float depth = fminf(
                fabsf(max0 - min1),
                fabsf(min0 - max1)
            );

            if (out && result.overlap_depth > depth) {
                result.overlap_depth = depth;
                result.entry_time = 0.0f;
                glm_vec3_copy(axis, result.normal);
            }
            continue;
        }

        float pdelta = glm_vec3_dot(axis, delta);

        float entry_dist = pdelta > 0.0f ?
            min1 - max0 :
            max1 - min0;

        float entry_time = entry_dist / pdelta;

        if (entry_time < 0.0f || entry_time > 1.0f) {
            return true;
        }

        if (out && entry_time < result.entry_time) {
            result.entry_time = entry_time;
            result.overlap_depth = 0.0f;
            glm_vec3_copy(axis, result.normal);
        }
    }

    memcpy(out, &result, sizeof(result));
    return false;
}

bool obb_vs_obb_static(
    obb_collider *a, obb_collider *b,
    transform *t0, transform *t1,
    sat_test_result *out
) {
    vec3 axes[15];

    gen_obb_normals(t0->rotation, axes);
    gen_obb_normals(t1->rotation, axes + 3);
    bb_cross_axes(axes, axes + 3, axes + 6);

    vec3 verts0[8]; gen_obb_vertices(a, t0, verts0);
    vec3 verts1[8]; gen_obb_vertices(b, t1, verts1);

    shape s0 = {8, verts0};
    shape s1 = {8, verts1};

    return !seperating_axis_static(&s0, &s1, axes, 15, out);
}

bool obb_vs_obb_swept(
    obb_collider *a, obb_collider *b,
    transform *t0, transform *t1,
    vec3 delta,
    sat_test_result *out
) {
    vec3 axes[15];

    gen_obb_normals(t0->rotation, axes);
    gen_obb_normals(t1->rotation, axes + 3);
    bb_cross_axes(axes, axes + 3, axes + 6);

    vec3 verts0[8]; gen_obb_vertices(a, t0, verts0);
    vec3 verts1[8]; gen_obb_vertices(b, t1, verts1);

    shape s0 = {8, verts0};
    shape s1 = {8, verts1};

    return !seperating_axis_swept(&s0, &s1, delta, axes, 15, out);
}