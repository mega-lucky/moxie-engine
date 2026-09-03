#include "./render_system.hpp"
#include "./process_calls.h"
#include <algorithm>
#include <cglm/cglm.h>
#include <iostream>

RenderSystem::RenderSystem(Engine &e) :
    world(e.WorldRegistry),
    MeshRender(world.GetComponentID("MeshRender")),
    MeshShape(world.GetComponentID("MeshShape")),
    Transform(world.GetComponentID("Transform"))
{
    glClearColor(1.0f,1.0f,0.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void RenderSystem::Update(double dt) {
    (void)dt;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_calls.clear();

    World::Query(world, {MeshRender, MeshShape, Transform}).Each([this](Entity entity){
        transform &t = world.GetComponent<transform>(entity, Transform);

        mesh_shape &m = world.GetComponent<mesh_shape>(entity, MeshShape);
        mesh_renderer &r = world.GetComponent<mesh_renderer>(entity, MeshRender);

        draw_call call = {
            .type = mesh_drawcall,
            .mesh = {
                .model = {0},
                .mesh = &m,
                .material = r.material,
            }
        };
        
        glm_quat_mat4(t.rotation, call.mesh.model);
        
        call.mesh.model[0][0] *= t.scale[0];
        call.mesh.model[1][1] *= t.scale[1];
        call.mesh.model[2][2] *= t.scale[2];

        call.mesh.model[3][0] = t.position[0];
        call.mesh.model[3][1] = t.position[1];
        call.mesh.model[3][2] = t.position[2];

        draw_calls.push_back(std::move(call));
    });

    process_calls(draw_calls.data(), draw_calls.size());
    glFinish();
}
