#include "engine.h"
#include "camera.h"
#include "physics.h"
#include "controls.h"
#include "transform.h"
#include "coroutine.h"
#include <stdio.h>

bool game_initialised = false;

void RenderSystemInit(void *state);
void RenderSystemUpdate(void *state);
void RegisterRenderSystem();
void RegisterRenderComponent();

int InitGame() {
    if (game_initialised) {
        return 0;
    }
    InitWindow();
    
    game_initialised = true;
    InitWorld();

    RegisterTransformComponent();
    RegisterRenderComponent();
    RegisterCameraComponent();
    RegisterRigidBodyComponent();
    RegisterCollisionComponent();
    RegisterInputState();
    RegisterControls();
    
    SystemDescription input_system = {
        .Name = "INPUT SYSTEM",
        .Phase = CYCLE_PHASE_INPUT,
        .Priority = 1000,
        .DataSize = 0,
        .Init = NULL,
        .Update = UpdateInputState,
    };
    RegisterSystem(input_system);
    SystemDescription control_system = {
        .Name = "CONTROL SYSTEM",
        .Phase = CYCLE_PHASE_INPUT,
        .Priority = 800,
        .DataSize = 0,
        .Init = NULL,
        .Update = UpdateControlState,
    };
    RegisterSystem(control_system);
    SystemDescription rigidbody_system = {
        .Name = "RIGIDBODY SYSTEM",
        .Phase = CYCLE_PHASE_PHYSICS,
        .Priority = 1000,
        .DataSize = sizeof(RigidBodySystemState),
        .Init = InitRigidBodies,
        .Update = UpdateRigidBodies,
    };
    RegisterSystem(rigidbody_system);
    SystemDescription collision_system = {
        .Name = "COLLISION SYSTEM",
        .Phase = CYCLE_PHASE_PHYSICS,
        .Priority = 10000,
        .DataSize = sizeof(CollisionSystemState),
        .Init = CollisionSystemInit,
        .Update = CollisionSystemUpdate
    };
    RegisterSystem(collision_system);
    RegisterRenderSystem();

    return 1;
}
void TerminateGame() {
    if (!game_initialised) {
        fprintf(stderr, "TRYING TO TERMINATE BEFORE INITIALISING THE GAME\n");
        return;
    }
    game_initialised = false;

    TerminateWorld();
    TerminateWindow();
}
void StartGameLoop() {
    while (!ShouldWindowClose()) {
        UpdateGame();
        CoroutineStep(GetDeltaTime());
        UpdateSystems();
    }
}