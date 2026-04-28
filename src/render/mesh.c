#include "render.h"
#include "memory.h"
#include "glad/glad.h"

static void bindvao(GLuint vao) {
    GLint bound;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &bound); GL_CHECK()
    if (bound == vao) { return; }

    glBindVertexArray(vao); GL_CHECK()
}

void UploadVertexData(Mesh *mesh, GLenum usage) {
    bindvao(mesh->vao);
    size_t vert_block_size = mesh->VertexCount * sizeof(MeshVertex);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo); GL_CHECK()
    glBufferData(GL_ARRAY_BUFFER, vert_block_size, mesh->VertexData, usage); GL_CHECK()
}
void UploadIndexData(Mesh *mesh, GLenum usage) {
    bindvao(mesh->vao);
    size_t index_data_size = sizeof(MeshIndex) * mesh->IndexCount;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo); GL_CHECK()
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data_size, mesh->Indices, usage); GL_CHECK()
}
void DrawVAO(GLuint vao, enum DrawType drawtype, unsigned int n) {
    bindvao(vao);
    if (drawtype == DRAW_ARRAYS) {
        glDrawArrays(GL_TRIANGLES, 0, n); GL_CHECK()
    } else if (drawtype == DRAW_ELEMENTS) {
        glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0); GL_CHECK()
    } else {
        return;
    }
}
void DrawVAOInstanced(GLuint vao, enum DrawType drawtype, unsigned int n, unsigned int instances) {
    bindvao(vao);
    if (drawtype == DRAW_ARRAYS) {
        glDrawArraysInstanced(GL_TRIANGLES, 0, n, instances); GL_CHECK()
    } else if (drawtype == DRAW_ELEMENTS) {
        glDrawElementsInstanced(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0, instances); GL_CHECK()
    } else {
        return;
    }
}

void UpdateAttributes(Mesh *mesh) {
    bindvao(mesh->vao);

    glVertexAttribPointer(
        AttribPositionIndex,
        3, GL_FLOAT, GL_FALSE,
        sizeof(MeshVertex),
        (void*)offsetof(MeshVertex, Position)
    ); GL_CHECK()
    glEnableVertexAttribArray(AttribPositionIndex);

    glVertexAttribPointer(
        AttribNormalIndex,
        3, GL_FLOAT, GL_FALSE,
        sizeof(MeshVertex),
        (void*)offsetof(MeshVertex, Normal)
    ); GL_CHECK()
    glEnableVertexAttribArray(AttribNormalIndex);

    glVertexAttribPointer(
        AttribColorIndex,
        4, GL_FLOAT, GL_FALSE,
        sizeof(MeshVertex),
        (void*)offsetof(MeshVertex, Colour)
    ); GL_CHECK()
    glEnableVertexAttribArray(AttribColorIndex);

    glVertexAttribPointer(
        AttribUVIndex,
        2, GL_FLOAT, GL_FALSE,
        sizeof(MeshVertex),
        (void*)offsetof(MeshVertex, UV)
    ); GL_CHECK()
    glEnableVertexAttribArray(AttribUVIndex);

}
Mesh *CreateMesh() {
    return CallocMem(1, sizeof(Mesh));
}
MeshVertex *CreateVertexBuffer(size_t vertex_count) {
    return CallocMem(vertex_count, sizeof(MeshVertex));
}
MeshIndex *CreateIndexBuffer(size_t index_count) {
    return CallocMem(index_count, sizeof(MeshIndex));
}
Material *CreateMaterial() {
    Material *material = AllocMem(sizeof(Material));
    material->Colour = Vec4One;
    material->Shader = NULL;
    material->Texture = NULL;
    material->UniformColour = true;

    return material;
}
GLuint GenVAO() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    return vao;
}
GLuint GenVBO() {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    return vbo;
}
GLuint GenEBO() {
    GLuint ebo;
    glGenBuffers(1, &ebo);
    return ebo;
}
void UploadMesh(Mesh *mesh, GLenum usage) {
    if (mesh->VertexCount > 0) {
        UploadVertexData(mesh, usage);
    }
    if (mesh->IndexCount > 0) {
        UploadIndexData(mesh, usage);
    }
    UpdateAttributes(mesh);
}

void DrawMesh(Mesh *mesh) {
    if (mesh->Indices != NULL && mesh->IndexCount > 0) {
        DrawVAO(mesh->vao, DRAW_ELEMENTS, mesh->IndexCount);
    } else {
        DrawVAO(mesh->vao, DRAW_ARRAYS, mesh->VertexCount);
    }
}

void DrawMeshInstanced(Mesh *mesh, unsigned int instances) {
    if (mesh->Indices != NULL && mesh->IndexCount > 0) {
        DrawVAOInstanced(mesh->vao, DRAW_ELEMENTS, mesh->IndexCount, instances);
    } else {
        DrawVAOInstanced(mesh->vao, DRAW_ARRAYS, mesh->VertexCount, instances);
    }
}
void DestroyMesh(Mesh *mesh) {
    if (mesh->VertexData != NULL && mesh->VertexCount > 0) {
        FreeMem(&mesh->VertexData);
    }
    if (mesh->Indices != NULL && mesh->IndexCount > 0) {
        FreeMem(&mesh->Indices);
    }
    FreeMem(&mesh);
}
AABB GetMeshAABB(Mesh *mesh) {
    AABB result = {
        .min = Vec3New(INFINITY, INFINITY, INFINITY),
        .max = Vec3New(-INFINITY, -INFINITY, -INFINITY),
    };

    for (size_t i = 0; i < mesh->VertexCount; i ++) {
        MeshVertex vertex = mesh->VertexData[i];

        result.min = Vec3Min(vertex.Position, result.min);
        result.max = Vec3Max(vertex.Position, result.max);
    }

    return result;
}
AABB TransformAABB(AABB aabb, Transform transform) {
    Vec3 min = aabb.min, max = aabb.max;
    Vec3 corners[8] = {
        Vec3New(max.x, max.y, max.z),
        Vec3New(min.x, max.y, max.z),
        Vec3New(max.x, min.y, max.z),
        Vec3New(max.x, max.y, min.z),
        Vec3New(min.x, min.y, min.z),
        Vec3New(max.x, min.y, min.z),
        Vec3New(min.x, max.y, min.z),
        Vec3New(min.x, min.y, max.z),
    };

    Vec3 p = TransformPoint(corners[0], transform);
    
    AABB result = { p, p };
    
    for (int i = 0; i < 8; i ++) {
        Vec3 p = TransformPoint(corners[i], transform);
        if (p.x < result.min.x) result.min.x = p.x;
        if (p.y < result.min.y) result.min.y = p.y;
        if (p.z < result.min.z) result.min.z = p.z;
        if (p.x > result.max.x) result.max.x = p.x;
        if (p.y > result.max.y) result.max.y = p.y;
        if (p.z > result.max.z) result.max.z = p.z;
    }

    return result;
}