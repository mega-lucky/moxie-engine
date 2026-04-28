#include "physics.h"
#include "vector.h"
#include "memory.h"

typedef struct ManifoldIndex {
    int start;
    int count;
} ManifoldIndex;

static ComponentMask collision_mask = 0;

CollisionManifold *list;
int *contact_indices;
size_t manifolds_count = 0;
ManifoldIndex *indices;

void CollisionSystemInit(void *data) {
    CollisionSystemState *desc = data;
    desc->entities = QueryEntities(collision_mask | MeshRenderMask() | TransformMask());
    list = AllocMem(sizeof(CollisionManifold) * 20);
    indices = AllocMem(sizeof(ManifoldIndex) * 20);
    contact_indices = AllocMem(sizeof(int) * 20);
}

void CollisionSystemUpdate(void *data) {
    manifolds_count = 0;
    for (int i = 0; i < 20; i ++) {
        indices[i].count = 0;
    }

    CollisionSystemState *desc = data;

    for (size_t i = 0; i < EntityList_len(desc->entities->Entities); i ++) {
        EntityID first = EntityList_get(desc->entities->Entities, i);

        for (size_t j = i+1; j < EntityList_len(desc->entities->Entities); j ++) {
            EntityID second = EntityList_get(desc->entities->Entities, j);

            CollisionManifold result = SweptSATTest(first, second);
            if (result.collided) {
                indices[i].count ++;
                indices[j].count ++;

                list[manifolds_count++] = result;
            }
        }
    }

    int offset = 0;
    for (size_t i = 0; i < EntityList_len(desc->entities->Entities); i ++) {
        EntityID entity = EntityList_get(desc->entities->Entities, i);

        indices[entity].start = offset;
        offset += indices[entity].count;
        indices[entity].count = 0;
    }
    for (size_t i = 0; i < manifolds_count; i ++) {
        EntityID a = list[i].a;
        EntityID b = list[i].b;

        contact_indices[indices[a].start + indices[a].count++] = i;
        contact_indices[indices[b].start + indices[b].count++] = i;
    }
}
const CollisionManifold *GetCollisionManifold(EntityID entity, size_t i) {
    if (!indices) { return NULL; }
    ManifoldIndex index = indices[entity];
    if (i >= index.count) {
        return NULL;
    }

    return &list[contact_indices[index.start + i]];
}
void InitCollisionFromMesh(Mesh *mesh, Collision *collision) {
    size_t nverts = mesh->VertexCount;
    size_t nnormals = mesh->IndexCount / 3;
    size_t nedges = mesh->IndexCount;
    collision->VertexCount = nverts;
    collision->NormalCount = nnormals;
    collision->EdgeCount = nedges;

    collision->VertexPositions = AllocMem(sizeof(Vec3) * nverts);
    collision->FaceNormals = AllocMem(sizeof(Vec3) * nnormals);
    collision->Edges = AllocMem(sizeof(Vec3) * nedges);

    for (size_t i = 0; i < nverts; i ++) {
        collision->VertexPositions[i] = mesh->VertexData[i].Position;
    }
    for (size_t i = 0; i < nnormals; i ++) {
        MeshIndex i0 = mesh->Indices[i * 3 + 0];
        MeshIndex i1 = mesh->Indices[i * 3 + 1];
        MeshIndex i2 = mesh->Indices[i * 3 + 2];

        Vec3 n0 = mesh->VertexData[i0].Normal;
        Vec3 n1 = mesh->VertexData[i1].Normal;
        Vec3 n2 = mesh->VertexData[i2].Normal;
        
        Vec3 normal_sum = Vec3Add(n0, Vec3Add(n1, n2));
        Vec3 face_normal = Vec3DivF(normal_sum, 3.0f);
        
        collision->FaceNormals[i] = face_normal;

        Vec3 p0 = mesh->VertexData[i0].Position;
        Vec3 p1 = mesh->VertexData[i1].Position;
        Vec3 p2 = mesh->VertexData[i2].Position;

        Vec3 e0 = Vec3Norm(Vec3Sub(p1, p0));
        Vec3 e1 = Vec3Norm(Vec3Sub(p2, p1));
        Vec3 e2 = Vec3Norm(Vec3Sub(p0, p2));

        collision->Edges[i * 3 + 0] = e0;
        collision->Edges[i * 3 + 1] = e1;
        collision->Edges[i * 3 + 2] = e2;
    }

    collision->aabb = mesh->aabb;
}
void collision_destrutor(void *data) {
    Collision *collision = (Collision *)data;
    FreeMem(&collision->VertexPositions);
    FreeMem(&collision->FaceNormals);
    FreeMem(&collision->Edges);
}
void RegisterCollisionComponent() {
    if (collision_mask != 0) { return; }
    collision_mask = RegisterComponent((ComponentDescription){
        .DataSize = sizeof(Collision),
        .DefaultValue = NULL,
        .Destructor = collision_destrutor
    });
}
ComponentMask CollisionMask() {
    return collision_mask;
}
Collision *GetCollision(EntityID entity) {
    return GetComponent(entity, collision_mask);
}