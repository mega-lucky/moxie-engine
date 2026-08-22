#include "./render_system.hpp"
#include "./process_calls.h"
#include <glm/gtc/type_ptr.hpp>

RenderSystem::RenderSystem(Engine &e) :
    mesh_query(e.WorldRegistry.NewQuery<transform, mesh_shape, mesh_renderer>()),
    world(e.WorldRegistry)
{
    glClearColor(1.0f,1.0f,0.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void RenderSystem::Update(double dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_calls.clear();

    for (auto entity : mesh_query) {
        transform &t = world.GetComponent<transform>(entity);
        glm::mat4 mrot = glm::mat4_cast(glm::make_quat(t.rotation));
        glm::mat4 mpos = glm::translate(glm::mat4(1.0f), glm::make_vec3(t.position));
        glm::mat4 mscale = glm::scale(glm::mat4(1.0f), glm::make_vec3(t.scale));

        glm::mat4 model = mpos * mrot * mscale;
        mesh_shape &m = world.GetComponent<mesh_shape>(entity);
        mesh_renderer &r = world.GetComponent<mesh_renderer>(entity);

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

        draw_calls.emplace_back(std::move(call));
    }

    process_calls(draw_calls.data(), draw_calls.size());
    glFinish();
}