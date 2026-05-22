#include <stdio.h>
#include "engine.h"
#include "transform.h"
#include "camera.h"
#include "physics.h"
#include "controls.h"
#include "stb_image.h"
#include <stdlib.h>

char *read_file(const char *path) {
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
EntityID orb = -1;
EntityID floor_entity = -1;

void PlayerSystemUpdate(void *data) {
    
    Player *player = GetSingleton(player_mask);
    ControlState *controls = GetControls();
    InputState *inputs = GetInputs();

    if (!player || !controls) { return; }

    Camera *camcomp = GetCamera(player->camera);
    camcomp->Frustrum.AspectRatio = GetWindowAspectRatio();

    Vec2 controls_cam_rot = controls->CameraRotation;
    controls_cam_rot.x = fclampf(
        controls_cam_rot.x,
        -RADIANS(80.0f),
        RADIANS(80.0f)
    );

    Transform *char_transform = GetTransform(player->character);
    Vec4 char_rotation = QuatFromAxisAngle(Vec3Up, controls_cam_rot.y);
    char_transform->Rotation = char_rotation;


    Vec4 cam_pitch = QuatFromAxisAngle(Vec3Up, controls_cam_rot.y);
    Vec4 cam_yaw = QuatFromAxisAngle(Vec3Right, controls_cam_rot.x);
    Vec4 cam_rot = Vec4Norm(QuatMul(cam_pitch, cam_yaw));

    Vec3 cam_forward = Vec3MulQ(Vec3Forward, cam_rot);
    Vec3 cam_right = Vec3MulQ(Vec3Right, cam_rot);

    Vec3 cam_lookat_position = Vec3Add(
        char_transform->Position,
        Vec3MulQ(player->camera_offset, char_rotation)
    );
    Vec3 cam_position = Vec3MulF(cam_forward, -12.0f);
    cam_position = Vec3Add(cam_position, cam_lookat_position);
    
    Transform *cam_transform = GetTransform(player->camera);
    cam_transform->Rotation = cam_rot;
    cam_transform->Position = cam_position;
    

    Vec3 char_move_direction = Vec3Add(
        Vec3MulF(cam_forward, controls->MoveDirection.z),
        Vec3MulF(cam_right, controls->MoveDirection.x)
    );
    char_move_direction.y = 0.0f;
    char_move_direction = Vec3Norm(char_move_direction);

    RigidBody *rigidbody = GetRigidBody(player->character);
    
    Vec3 XZ_velocity = Vec3MulF(Vec3Norm(char_move_direction), 10.0f);
    
    rigidbody->Velocity.x = XZ_velocity.x;
    rigidbody->Velocity.z = XZ_velocity.z;

    if (IS_INPUT_STARTED(inputs->KeyButtons[' '])) {
        rigidbody->Velocity.y = 50.0f;
    }
}

int main() {
    InitGame();
    char *vert_src = read_file("assets/shaders/default_vert.glsl");
    char *frag_src = read_file("assets/shaders/default_frag.glsl");

    Material *default_material = CreateMaterial();
    default_material->Shader = CreateShader(vert_src, frag_src);
    default_material->UniformColour = true;
    default_material->Colour = Vec4New(0.4f, 0.7f, 0.6f, 1.0f);

    Mesh *cube_mesh = CubeMesh();
    UploadMesh(cube_mesh, STATIC_DRAW);
    cube_mesh->Material = default_material;
    
    Mesh *sphere_mesh = SphereMesh(0.5f, 6, 12);
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

    character = NewEntity();
    {
        EntityGiveComponents(character, basepart);
        Transform *transform = GetTransform(character);
        transform->Scale = Vec3New(2.0f, 4.0f, 2.0f);
        
        MeshRender *mesh_render = GetMeshRender(character);
        mesh_render->Mesh = cube_mesh;
        mesh_render->Colour = Vec4New(0.4f, 0.7f, 0.6f, 1.0f);
        mesh_render->Material = default_material;
        
        transform->Position = Vec3New(-20.0f, 0.0f, -15.0f);

        Collision *collision = GetCollision(character);
        InitCollisionFromMesh(mesh_render->Mesh, collision);

        GetRigidBody(character)->Acceleration.y = -100.0f;
    }
    {
        floor_entity = NewEntity();
        EntityGiveComponents(floor_entity, basepart);

        Transform *transform = GetTransform(floor_entity);
        transform->Scale = Vec3New(100.0f, 2.0f, 100.0f);
        transform->Position = Vec3New(0.0f, -10.0f, 0.0f);
        
        MeshRender *mesh_render = GetMeshRender(floor_entity);
        mesh_render->Mesh = cube_mesh;
        mesh_render->Colour = Vec4New(0.7f, 0.3f, 0.4f, 1.0f);
        mesh_render->Material = default_material;

        Collision *collision = GetCollision(floor_entity);
        InitCollisionFromMesh(mesh_render->Mesh, collision);

        GetRigidBody(floor_entity)->Static = true;
    }
    Material *goat_material = CreateMaterial();
    Texture *goat_texture = GenTextureFromFileName("assets/image/goat.jpg");
    UploadTexture(goat_texture);
    goat_material->Texture = goat_texture;
    goat_material->Shader = default_material->Shader;
    {
        orb = NewEntity();
        EntityGiveComponents(orb, basepart);

        Transform *transform = GetTransform(orb);
        transform->Scale = Vec3New(15.0f,15.0f,15.0f);
        transform->Position = Vec3New(0.0f, 0.0f, 0.0f);
        transform->Rotation = QuatFromAxisAngle(Vec3Up, RADIANS(45.0f));
        
        MeshRender *mesh_render = GetMeshRender(orb);
        mesh_render->Mesh = cube_mesh;
        mesh_render->Colour = Vec4New(1.0f, 1.0f, 1.0f, 1.0f);
        mesh_render->Material = goat_material;

        Collision *collision = GetCollision(orb);
        InitCollisionFromMesh(mesh_render->Mesh, collision);

        RigidBody *rigidbody = GetRigidBody(orb);
        rigidbody->Acceleration.y = -100.0f;
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
    DestroyMaterial(goat_material);
    DestroyTexture(goat_texture);
    TerminateGame();
    return 0;
}