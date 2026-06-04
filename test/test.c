#include <stdio.h>
#include "moxie/engine.h"
#include "moxie/transform.h"
#include "moxie/camera.h"
#include "moxie/physics.h"
#include "moxie/controls.h"
#include <stdlib.h>

bool gjk_overlap(EntityID first, EntityID second);

static char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    char *buf = malloc(len + 1);
    fread(buf, 1, len, f);
    buf[len] = '\0';

    fclose(f);
    return buf;
}

struct Player {
    EntityID camera;
    EntityID character;
    Vec3 camera_offset;
};
typedef struct Player Player;

ComponentMask player_mask = 0;

EntityID character = -1;
EntityID floor_entity = -1;

void PlayerSystemUpdate(void *data) {
    (void)data;
    Player *player = GetSingleton(player_mask);
    ControlState *controls = GetControls();
    
    if (!player || !controls) { return; }
    
    MeshRender *mesh_render = GetMeshRender(player->character);
    Camera *camcomp = GetCamera(player->camera);

    camcomp->Frustrum.AspectRatio = GetWindowAspectRatio();

    Vec2 controls_cam_rot = controls->CameraRotation;
    controls_cam_rot.x = fclampf(
        controls_cam_rot.x,
        -RADIANS(80.0f),
        RADIANS(80.0f)
    );

    Transform *char_transform = GetTransform(player->character);

    Vec3 cam_position = Vec3Add(
        char_transform->Position,
        Vec3New(0, 0, 12.0f)
    );
    
    Transform *cam_transform = GetTransform(player->camera);
    cam_transform->Position = cam_position;
    

    Vec3 char_move_direction = Vec3Add(
        Vec3MulF(Vec3Up, controls->MoveDirection.z),
        Vec3MulF(Vec3Right, controls->MoveDirection.x)
    );
    char_move_direction = Vec3Norm(char_move_direction);

    RigidBody *rigidbody = GetRigidBody(player->character);
    rigidbody->Velocity = Vec3MulF(char_move_direction, 10.0f);

    if (gjk_overlap(character, floor_entity)) {
        mesh_render->Colour = Vec4New(1.0f, 0.0f, 0.0f, 1.0f);
    } else {
        mesh_render->Colour = Vec4New(0.0f, 0.0f, 1.0f, 1.0f);
    }
}

int main() {
    InitGame();
    char *vert_src = read_file("test/assets/shaders/default_vert.glsl");
    char *frag_src = read_file("test/assets/shaders/default_frag.glsl");

    Material *default_material = CreateMaterial();
    default_material->Shader = CreateShader(vert_src, frag_src);
    default_material->UniformColour = true;
    default_material->Colour = Vec4New(0.4f, 0.7f, 0.6f, 1.0f);

    Mesh *cube_mesh = CubeMesh();
    UploadMesh(cube_mesh, STATIC_DRAW);
    cube_mesh->Material = default_material;
    
    Mesh *sphere_mesh = SphereMesh(0.5f, 12, 24);
    UploadMesh(sphere_mesh, STATIC_DRAW);
    sphere_mesh->Material = default_material;

    free(vert_src);
    free(frag_src);

    EntityID camera = NewEntity();
    EntityGiveComponents(camera, CameraMask() | TransformMask());
    Camera *camcomp = GetCamera(camera);
    camcomp->Projection = PERSPECTIVE_PROJECTION;
    camcomp->Near = 0.1f;
    camcomp->Far = 500.0f;
    camcomp->Frustrum = (CameraFrustrum){ RADIANS(70.0f), GetWindowAspectRatio() };

    ComponentMask basepart =
        MeshRenderMask() | TransformMask() |
        CollisionMask() | RigidBodyMask();

    {
        character = NewEntity();
        EntityGiveComponents(character, basepart);
        Transform *transform = GetTransform(character);
        transform->Scale = Vec3New(4.0f, 4.0f, 4.0f);
        transform->Rotation = QuatFromAxisAngle(Vec3Up, RADIANS(45.0f));
        
        MeshRender *mesh_render = GetMeshRender(character);
        mesh_render->Mesh = cube_mesh;
        mesh_render->Colour = Vec4New(0.4f, 0.7f, 0.6f, 1.0f);
        mesh_render->Material = default_material;

        Collision *collision = GetCollision(character);
        InitCollisionFromMesh(mesh_render->Mesh, collision);
        collision->NonCollidable = true;
    }
    {
        floor_entity = NewEntity();
        EntityGiveComponents(floor_entity, basepart);

        Transform *transform = GetTransform(floor_entity);
        transform->Scale = Vec3New(10.0f, 2.0f, 2.0f);
        
        MeshRender *mesh_render = GetMeshRender(floor_entity);
        mesh_render->Mesh = cube_mesh;
        mesh_render->Colour = Vec4New(0.7f, 0.3f, 0.4f, 1.0f);
        mesh_render->Material = default_material;

        GetRigidBody(floor_entity)->Static = true;

        Collision *collision = GetCollision(floor_entity);
        InitCollisionFromMesh(cube_mesh, collision);
        collision->NonCollidable = true;
    }

    player_mask = RegisterSingleton((ComponentDescription){
        .DataSize = sizeof(Player),
        .Init = NULL,
        .Del = NULL
    });

    Player *player = GetSingleton(player_mask);
    player->camera = camera;
    player->character = character;
    player->camera_offset = Vec3New(2.0f, 1.0f, 0.0f);

    RegisterSystem((SystemDescription){
        .Name = "Player System",
        .DataSize = 0,
        .Init = NULL,
        .Phase = CYCLE_PHASE_PRERENDER,
        .Priority = 100,
        .Update = PlayerSystemUpdate
    });
    
    StartGameLoop();

    DestroyMesh(cube_mesh);
    DestroyMesh(sphere_mesh);
    DestroyShader(default_material->Shader);
    DestroyMaterial(default_material);
    TerminateGame();
    return 0;
}