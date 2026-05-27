#ifndef RENDERHANDLER_H
#define RENDERHANDLER_H

#include "moxmath/moxmath.h"
#include "moxie/transform.h"
#include "moxie/world.h"

#include <stdbool.h>
#include <stddef.h>

#define AttribPositionIndex  0
#define AttribNormalIndex    1
#define AttribColorIndex     2
#define AttribUVIndex        3

#define AttribPositionMask   (1 << AttribPositionIndex)
#define AttribNormalMask     (1 << AttribNormalIndex)
#define AttribColorMask      (1 << AttribColorIndex)

#include "stdio.h"
#define GL_CHECK() do {int e = glGetError(); if (e) printf("OPENGL: Error %d at in %s at line %d\n", e, __FILE__, __LINE__); } while (0);

typedef unsigned int MeshIndex;

enum DrawType {
    DRAW_ARRAYS,
    DRAW_ELEMENTS,
};

enum DrawType2 {
    STATIC_DRAW = 0x88E4,
};

enum TextureWrapMode {
    TEXTURE_WRAP_REPEAT,
    TEXTURE_WRAP_MIRRORED_REPEAT,
    TEXTURE_WRAP_EDGE_CLAMP,
    TEXTURE_WRAP_BORDER_CLAMP
};

enum TextureFilterMode {
    TEXTURE_FILTER_LINEAR,
    TEXTURE_FILTER_NEAREST,
    TEXTURE_FILTER_MIPMAP_LINEAR,
    TEXTURE_FILTER_MIPMAP_NEAREST,
};

enum TextureType {
    TEXTURE_TYPE_2D,
    TEXTURE_TYPE_CUBE_MAP,
    TEXTURE_TYPE_3D,
    TEXTURE_TYPE_2D_ARRAY
};

typedef struct Uniform {
    const char *Name;
    int Location;
} Uniform;

typedef struct ShaderProgram {
    unsigned int ID;
    Uniform *Uniforms;
    unsigned int UniformsCount;
} ShaderProgram;

typedef struct Texture {
    enum TextureType type;
    enum TextureFilterMode min_filter;
    enum TextureFilterMode mag_filter;
    enum TextureWrapMode wrap_mode;
    const unsigned char *data;
    int width, height, nChannels;
    unsigned int ID;
} Texture;

typedef struct Material {
    ShaderProgram *Shader;
    Vec4 Colour;
    Texture *Texture;
    bool UniformColour;
} Material;

typedef struct AABB { Vec3 min, max; } AABB;

typedef struct MeshVertex {
    Vec3 Position;
    Vec3 Normal;
    Vec4 Colour;
    Vec2 UV;
} MeshVertex;

typedef struct Mesh {
    MeshVertex *VertexData;
    size_t VertexCount;
    AABB aabb;

    MeshIndex *Indices;
    size_t IndexCount;

    unsigned int vao, vbo, ebo;

    Material *Material;
} Mesh;

typedef struct MeshRender {
    Mesh *Mesh;
    bool DontRender;
    Vec4 Colour;
    Material *Material;
} MeshRender;

struct GameObject;
Mesh *CreateMesh();
MeshVertex *CreateVertexBuffer(size_t vertex_count);
MeshIndex *CreateIndexBuffer(size_t index_count);
Material *CreateMaterial();
unsigned int GenVAO();
unsigned int GenVBO();
unsigned int GenEBO();
void UploadMesh(Mesh *mesh, unsigned int usage);
void DrawVAO(unsigned int vao, enum DrawType drawtype, unsigned int n);
void DrawVAOInstanced(unsigned int vao, enum DrawType drawtype, unsigned int n, unsigned int instances);
void DrawMesh(Mesh *mesh);
void DrawMeshInstanced(Mesh *mesh, unsigned int instances);
void DestroyMesh(Mesh *mesh);
void DestroyMaterial(Material *material);
void DestroyShader(ShaderProgram *shader);
void DestroyTexture(Texture *texture);
ShaderProgram *CreateShader(const char *vert_src, const char *frag_src);
int GetUniformLocation(ShaderProgram *shader, const char *name);
AABB GetMeshAABB(Mesh *mesh);
AABB TransformAABB(AABB aabb, Transform transform);
Mesh *CubeMesh();
Mesh *SphereMesh(float radius, size_t stacks, size_t sectors);
Texture *GenTextureFromFileName(const char *filename);
void UploadTexture(Texture *texture);

ComponentMask MeshRenderMask();
MeshRender *GetMeshRender(EntityID entity);

#endif