#include "./types.h"

void obb_axis_min_max(vec3 *verts, vec3 axis, float *min, float *max) {
    *min = FLT_MAX; *max = -FLT_MAX;

    for (int i = 0; i < 8; i ++) {
        float projection = glm_vec3_dot(axis, verts[i]);
        *min = fminf(*min, projection);
        *max = fmaxf(*max, projection);
    }
}

bool obb_vs_obb(obb_collider *a, obb_collider *b, vec3 p0, vec3 p1) {
    mat3 r0, r1;
    glm_quat_mat3(a->r, r0);
    glm_quat_mat3(b->r, r1);

    vec3 c0, s0;
    glm_vec3_add(a->min, a->max, c0);
    glm_vec3_divs(c0, 2.0f, c0);
    glm_mat3_mulv(r0, c0, c0);
    glm_vec3_add(c0, p0, c0);

    glm_vec3_sub(a->max, a->min, s0);
    glm_vec3_divs(s0, 2.0f, s0);
    
    vec3 c1, s1;
    glm_vec3_add(b->min, b->max, c1);
    glm_vec3_divs(c1, 2.0f, c1);
    glm_mat3_mulv(r1, c1, c1);
    glm_vec3_add(c1, p1, c1);
    
    glm_vec3_sub(b->max, b->min, s1);
    glm_vec3_divs(s1, 2.0f, s1);

    vec3 axes[15];
    glm_quat_rotatev(a->r, GLM_XUP, axes[0]);
    glm_quat_rotatev(a->r, GLM_YUP, axes[1]);
    glm_quat_rotatev(a->r, GLM_ZUP, axes[2]);

    glm_quat_rotatev(b->r, GLM_XUP, axes[3]);
    glm_quat_rotatev(b->r, GLM_YUP, axes[4]);
    glm_quat_rotatev(b->r, GLM_ZUP, axes[5]);

    for (int i = 0; i < 3; i ++) {
        for (int j = 0; j < 3; j ++) {
            glm_vec3_cross(axes[i], axes[j + 3], axes[6 + j * 3 + i]);
        }
    }

    vec3 verts0[8] = {
        { s0[0],  s0[1],  s0[2]},
        {-s0[0],  s0[1],  s0[2]},
        { s0[0], -s0[1],  s0[2]},
        { s0[0],  s0[1], -s0[2]},
        { s0[0], -s0[1], -s0[2]},
        {-s0[0],  s0[1], -s0[2]},
        {-s0[0], -s0[1],  s0[2]},
        {-s0[0], -s0[1], -s0[2]},
    };

    vec3 verts1[8] = {
        { s1[0],  s1[1],  s1[2]},
        {-s1[0],  s1[1],  s1[2]},
        { s1[0], -s1[1],  s1[2]},
        { s1[0],  s1[1], -s1[2]},
        { s1[0], -s1[1], -s1[2]},
        {-s1[0],  s1[1], -s1[2]},
        {-s1[0], -s1[1],  s1[2]},
        {-s1[0], -s1[1], -s1[2]},
    };

    for (int i = 0; i < 8; i ++) {
        glm_mat3_mulv(r0, verts0[i], verts0[i]);
        glm_vec3_add(verts0[i], c0, verts0[i]);

        glm_mat3_mulv(r1, verts1[i], verts1[i]);
        glm_vec3_add(verts1[i], c1, verts1[i]);
    }

    for (int i = 0; i < 15; i ++) {
        if (glm_vec3_norm2(axes[i]) < GLM_FLT_EPSILON) {
            continue;
        }

        float *axis = axes[i];
        float min0, max0, min1, max1;

        obb_axis_min_max(verts0, axis, &min0, &max0);
        obb_axis_min_max(verts1, axis, &min1, &max1);

        if (max0 < min1 || max1 < min0) {
            return false;
        }
    }

    return true;
}