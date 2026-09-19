#include "./types.h"
#include <string.h>

static inline void gen_obb_vertices(obb_collider *obb, vec3 p, vec3 *out) {
    mat3 r; glm_quat_mat3(obb->r, r);

    vec3 c, s;
    glm_vec3_add(obb->min, obb->max, c);
    glm_vec3_divs(c, 2.0f, c);
    glm_mat3_mulv(r, c, c);
    glm_vec3_add(c, p, c);

    glm_vec3_sub(obb->max, obb->min, s);
    glm_vec3_divs(s, 2.0f, s);

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

static inline void gen_obb_normals(obb_collider *obb, vec3 *out) {
    glm_quat_rotatev(obb->r, GLM_XUP, out[0]);
    glm_quat_rotatev(obb->r, GLM_YUP, out[1]);
    glm_quat_rotatev(obb->r, GLM_ZUP, out[2]);
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

static inline bool seperating_axis(shape *a, shape *b, vec3 *axes, size_t n_axes) {
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
    }

    return false;
}

bool obb_vs_obb(obb_collider *a, obb_collider *b, vec3 p0, vec3 p1) {
    vec3 axes[15];

    gen_obb_normals(a, axes);
    gen_obb_normals(b, axes + 3);
    bb_cross_axes(axes, axes + 3, axes + 6);

    vec3 verts0[8]; gen_obb_vertices(a, p0, verts0);
    vec3 verts1[8]; gen_obb_vertices(b, p1, verts1);

    shape s0 = {8, verts0};
    shape s1 = {8, verts1};

    return !seperating_axis(&s0, &s1, axes, 15);
}