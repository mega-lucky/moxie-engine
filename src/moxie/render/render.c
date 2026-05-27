#include "glad/glad.h"
#include "moxie/render.h"
#include "moxie/camera.h"
#include "moxie/window.h"
#include "moxlib/memory.h"

typedef struct RenderSystemState {
    const EntityQuery *renderables;
    const EntityQuery *cameras;
} RenderSystemState;

static ComponentMask render_mask = 0;
static bool render_system_init = false;

static void window_resize(int width, int height) {
    glViewport(0, 0, width, height); GL_CHECK()
}
static void RenderSystemInit(void *data) {
    RenderSystemState *state = data;
    
    state->renderables = QueryEntities(render_mask | TransformMask());
    state->cameras = QueryEntities(CameraMask() | TransformMask());
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); GL_CHECK()
    glEnable(GL_DEPTH_TEST); GL_CHECK()
    glDisable(GL_CULL_FACE); GL_CHECK()
    glViewport(0, 0, GetWindowWidth(), GetWindowHeight()); GL_CHECK()
    glEnable(GL_CULL_FACE); GL_CHECK()
    glCullFace(GL_BACK); GL_CHECK()
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    BindToWindowResize(window_resize);
}
static void RenderSystemUpdate(void *data) {
    RenderSystemState *state = data;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (size_t i = 0; i < EntityList_len(state->cameras->Entities); i ++) {
        EntityID camera = EntityList_get(state->cameras->Entities, i);

        Mat4x4 view = GetViewMatrix(camera);
        Mat4x4 proj = GetProjectionMatrix(camera);

        for (size_t j = 0; j < EntityList_len(state->renderables->Entities); j ++) {
            EntityID renderable = EntityList_get(state->renderables->Entities, j);
            MeshRender *mesh_render = GetMeshRender(renderable);
            Transform *transform = GetTransform(renderable);

            Mesh *mesh = mesh_render->Mesh;
            if (!mesh) { continue; }

            ShaderProgram *shader = mesh->Material->Shader;
            glUseProgram(shader->ID); GL_CHECK()
            GLuint model_loc = GetUniformLocation(shader, "u_model"); GL_CHECK()
            GLuint view_loc = GetUniformLocation(shader, "u_view"); GL_CHECK()
            GLuint proj_loc = GetUniformLocation(shader, "u_projection"); GL_CHECK()
            GLuint colour_loc = GetUniformLocation(shader, "u_colour"); GL_CHECK()
            GLuint lightpos_loc = GetUniformLocation(shader, "u_lightpos"); GL_CHECK()
            GLuint texture_loc = GetUniformLocation(shader, "u_texture"); GL_CHECK()
            GLuint is_textured_loc = GetUniformLocation(shader, "u_istextured"); GL_CHECK();
            Vec3 lightpos = Vec3New(200.0f, 100.0f, 150.0f);

            Mat4x4 model = Mat4FromTransform(*transform);

            glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.components); GL_CHECK() 
            glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.components); GL_CHECK()
            glUniformMatrix4fv(proj_loc, 1, GL_FALSE, proj.components); GL_CHECK()
            glUniform4fv(colour_loc, 1, mesh_render->Colour.components); GL_CHECK()
            glUniform3fv(lightpos_loc, 1, lightpos.components);
            if (mesh_render->Material && mesh_render->Material->Texture) {
                glUniform1i(is_textured_loc, GL_TRUE); GL_CHECK()
                glUniform1i(texture_loc, 0); GL_CHECK()
            } else {
                glUniform1i(is_textured_loc, GL_FALSE);  GL_CHECK()
            }

            DrawMesh(mesh);
        }
    }

    glFinish();
    SwapFrameBuffers();
}

void RegisterRenderComponent() {
    if (render_mask != 0) { return; }
    render_mask = RegisterComponent((ComponentDescription){
        .DataSize = sizeof(MeshRender),
        .Init = NULL,
        .Del = NULL
    });
}
void RegisterRenderSystem() {
    if (render_system_init) { return; }
    RegisterSystem((SystemDescription){
        .Name = "RENDER SYSTEM",
        .Phase = CYCLE_PHASE_RENDER,
        .Priority = 1000,
        .DataSize = sizeof(struct RenderSystemState),
        .Init = RenderSystemInit,
        .Update = RenderSystemUpdate,
    });
}
ComponentMask MeshRenderMask() {
    return render_mask;
}
MeshRender *GetMeshRender(EntityID entity) {
    return GetComponent(entity, render_mask);
}