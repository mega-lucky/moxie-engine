#include "./render_system.hpp"
#include "./process_calls.h"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_calls.clear();

    World::Query(world, {MeshRender, MeshShape, Transform}).Each([this](Entity entity){
        transform &t = world.GetComponent<transform>(entity, Transform);
        glm::mat4 mrot = glm::mat4_cast(glm::make_quat(t.rotation));
        glm::mat4 mpos = glm::translate(glm::mat4(1.0f), glm::make_vec3(t.position));
        glm::mat4 mscale = glm::scale(glm::mat4(1.0f), glm::make_vec3(t.scale));

        glm::mat4 model = mpos * mrot * mscale;
        mesh_shape &m = world.GetComponent<mesh_shape>(entity, MeshShape);
        mesh_renderer &r = world.GetComponent<mesh_renderer>(entity, MeshRender);

        draw_call call = {
            .type = mesh_drawcall,
            .mesh = {
                .model = {},
                .mesh = &m,
                .material = r.material,
            }
        };

        float *start = glm::value_ptr(model);
        float *end = start + 16;

        std::copy(start, end, call.mesh.model);

        draw_calls.push_back(std::move(call));
    });

    process_calls(draw_calls.data(), draw_calls.size());
    glFinish();
}
