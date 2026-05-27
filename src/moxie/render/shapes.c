#include "moxie/render.h"
#include "moxlib/memory.h"

#include "glad/glad.h"
#include <math.h>
#include <string.h>

static const Vec3 CUBE_VERTEX_POSITIONS[] = {
    Vec3New(0.5f, 0.5f, 0.5f),   Vec3New(0.5f, -0.5f, 0.5f),  
    Vec3New(0.5f, -0.5f, -0.5f), Vec3New(0.5f, 0.5f, -0.5f),

    Vec3New(-0.5f, 0.5f, -0.5f), Vec3New(-0.5f, -0.5f, -0.5f),
    Vec3New(-0.5f, -0.5f, 0.5f), Vec3New(-0.5f, 0.5f, 0.5f),

    Vec3New(0.5f, 0.5f, 0.5f),   Vec3New(0.5f, 0.5f, -0.5f),  
    Vec3New(-0.5f, 0.5f, -0.5f), Vec3New(-0.5f, 0.5f, 0.5f),
    
    Vec3New(-0.5f, -0.5f, 0.5f), Vec3New(-0.5f, -0.5f, -0.5f),
    Vec3New(0.5f, -0.5f, -0.5f), Vec3New(0.5f, -0.5f, 0.5f),

    Vec3New(-0.5f, 0.5f, 0.5f),  Vec3New(-0.5f, -0.5f, 0.5f), 
    Vec3New(0.5f, -0.5f, 0.5f),  Vec3New(0.5f, 0.5f, 0.5f),

    Vec3New(0.5f, 0.5f, -0.5f),  Vec3New(0.5f, -0.5f, -0.5f), 
    Vec3New(-0.5f, -0.5f, -0.5f),Vec3New(-0.5f, 0.5f, -0.5f), 
};

static const Vec3 CUBE_VERTEX_NORMALS[] = {
    Vec3New(1.0f, 0.0f, 0.0f),  Vec3New(1.0f, 0.0f, 0.0f),
    Vec3New(1.0f, 0.0f, 0.0f),  Vec3New(1.0f, 0.0f, 0.0f),

    Vec3New(-1.0f, 0.0f, 0.0f), Vec3New(-1.0f, 0.0f, 0.0f),
    Vec3New(-1.0f, 0.0f, 0.0f), Vec3New(-1.0f, 0.0f, 0.0f),

    Vec3New(0.0f, 1.0f, 0.0f),  Vec3New(0.0f, 1.0f, 0.0f),
    Vec3New(0.0f, 1.0f, 0.0f),  Vec3New(0.0f, 1.0f, 0.0f),

    Vec3New(0.0f, -1.0f, 0.0f), Vec3New(0.0f, -1.0f, 0.0f),
    Vec3New(0.0f, -1.0f, 0.0f), Vec3New(0.0f, -1.0f, 0.0f),

    Vec3New(0.0f, 0.0f, 1.0f),  Vec3New(0.0f, 0.0f, 1.0f),
    Vec3New(0.0f, 0.0f, 1.0f),  Vec3New(0.0f, 0.0f, 1.0f),

    Vec3New(0.0f, 0.0f, -1.0f), Vec3New(0.0f, 0.0f, -1.0f),
    Vec3New(0.0f, 0.0f, -1.0f), Vec3New(0.0f, 0.0f, -1.0f),
};

static const Vec2 CUBE_VERTEX_UV_MAP[] = {
    Vec2New(0.0f, 1.0f), Vec2New(0.0f, 0.0f),
    Vec2New(1.0f, 0.0f), Vec2New(1.0f, 1.0f),

    Vec2New(0.0f, 1.0f), Vec2New(0.0f, 0.0f),
    Vec2New(1.0f, 0.0f), Vec2New(1.0f, 1.0f),

    Vec2New(0.0f, 1.0f), Vec2New(0.0f, 0.0f),
    Vec2New(1.0f, 0.0f), Vec2New(1.0f, 1.0f),

    Vec2New(0.0f, 1.0f), Vec2New(0.0f, 0.0f),
    Vec2New(1.0f, 0.0f), Vec2New(1.0f, 1.0f),

    Vec2New(0.0f, 1.0f), Vec2New(0.0f, 0.0f),
    Vec2New(1.0f, 0.0f), Vec2New(1.0f, 1.0f),
    
    Vec2New(0.0f, 1.0f), Vec2New(0.0f, 0.0f),
    Vec2New(1.0f, 0.0f), Vec2New(1.0f, 1.0f),
};

static const unsigned int CUBE_INDICES[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20
};

const size_t CUBE_VERT_COUNT = 24;
const size_t CUBE_IND_COUNT = 36;

Mesh *CubeMesh() {
    Mesh *cube = CreateMesh();
    cube->vao = GenVAO();
    cube->vbo = GenVBO();
    cube->ebo = GenEBO();

    cube->IndexCount = CUBE_IND_COUNT;
    cube->Indices = CreateIndexBuffer(cube->IndexCount);

    cube->VertexCount = CUBE_VERT_COUNT;
    cube->VertexData = CreateVertexBuffer(cube->VertexCount);

    cube->aabb.min = Vec3New(INFINITY, INFINITY, INFINITY);
    cube->aabb.max = Vec3New(-INFINITY, -INFINITY, -INFINITY);

    for (size_t i = 0; i < cube->VertexCount; i ++) {
        MeshVertex *vertex = &cube->VertexData[i];

        Vec3 position = CUBE_VERTEX_POSITIONS[i];
        vertex->Position = position;

        cube->aabb.min = Vec3Min(cube->aabb.min, position);
        cube->aabb.max = Vec3Max(cube->aabb.max, position);
        
        Vec3 normal = CUBE_VERTEX_NORMALS[i];
        vertex->Normal = normal;

        Vec4 colour = Vec4One;
        vertex->Colour = colour;

        Vec2 uv = CUBE_VERTEX_UV_MAP[i];
        vertex->UV = uv;
    }
    memcpy(cube->Indices, CUBE_INDICES, cube->IndexCount * sizeof(MeshIndex));

    return cube;
}

Mesh *SphereMesh(float radius, size_t stacks, size_t sectors) {
    Mesh *sphere = CreateMesh();
    sphere->vao = GenVAO();
    sphere->vbo = GenVBO();
    sphere->ebo = GenEBO();
    
    sphere->VertexCount = (stacks+1) * (sectors+1);
    sphere->VertexData = CreateVertexBuffer(sphere->VertexCount);

    sphere->IndexCount = stacks * sectors * 6;
    sphere->Indices = CreateIndexBuffer(sphere->IndexCount);
    
    sphere->aabb.min = Vec3New(INFINITY, INFINITY, INFINITY);
    sphere->aabb.max = Vec3New(-INFINITY, -INFINITY, -INFINITY);

    size_t vert = 0;
    for (size_t i = 0; i <= stacks; i ++) {
        for (size_t j = 0; j <= sectors; j ++) {
            MeshVertex *vertex = &sphere->VertexData[vert++];

            float pitch = (float)i/stacks * PI_F;
            float yaw = (float)j/sectors * PI_F * 2.0f;

            Vec3 position =  Vec3New(
                radius * sinf(pitch) * sinf(yaw),
                radius * cosf(pitch),
                radius * sinf(pitch) * cosf(yaw)
            );
            vertex->Position = position;

            sphere->aabb.min = Vec3Min(sphere->aabb.min, position);
            sphere->aabb.max = Vec3Max(sphere->aabb.max, position);

            Vec3 normal = Vec3Norm(position);
            vertex->Normal = normal;

            Vec4 colour = Vec4One;
            vertex->Colour = colour;
        }
    }

    size_t index_maxn = 0;
    for (size_t i = 0; i < stacks; i ++) {
        unsigned int k1 = i * (sectors + 1);
        unsigned int k2 = k1 + sectors + 1;

        for (size_t j = 0; j < sectors; j ++, k1++, k2++) {
            if (i != 0) {
                sphere->Indices[index_maxn++] = k1;
                sphere->Indices[index_maxn++] = k2;
                sphere->Indices[index_maxn++] = k1 + 1;
            }

            if (i != (stacks - 1)) {
                sphere->Indices[index_maxn++] = k1 + 1;
                sphere->Indices[index_maxn++] = k2;
                sphere->Indices[index_maxn++] = k2 + 1;
            }
        }
    }

    return sphere;
}