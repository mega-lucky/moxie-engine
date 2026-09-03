#include "./process_calls.h"
#include "./shader.h"
#include "./mesh.h"
#include <cglm/cglm.h>

typedef struct text_data {
    float pos[2];
    float uv[2];
    char colour[4];
} text_data;

static shader_data text_shader = {0};
static mesh_shape text_mash = {0};
static text_data *text_data_buffer = NULL;
static size_t n_text_vertices = 0;

void init_drawcall_proccessor() {
    init_shader_from_file(&text_shader, "assets/shaders/text_vert.glsl", "assets/shaders/text_frag.glsl");

    vert_attrib_data text_attribs[3] = {
        {.size=2,.type=ATTRIB_F32,.normalised=false},
        {.size=2,.type=ATTRIB_F32,.normalised=false},
        {.size=4,.type=ATTRIB_U8,.normalised=true}
    };
    attrib_layout text_layout = {
        .attribs = text_attribs,
        .n_attribs = 3,
        .stride = 12
    };
    xinit_mesh(&text_mash, NULL, 0, NULL, 0, &text_layout);
}

void run_draw_call(const draw_call *call, const mat4 view, const mat4 proj) {
    switch (call->type) {
        case mesh_drawcall: {
            glBindVertexArray(call->mesh.mesh->VAO);
            if (!call->mesh.material || !call->mesh.mesh) {
                return;
            }

            shader_data *shader = call->mesh.material->shader;
            texture_data *texture = call->mesh.material->textures[0];
            const float *model = &call->mesh.model[0][0];

            glUseProgram(shader->id);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture->id);

            glUniform1i(glGetUniformLocation(shader->id, "utexture"), 0);
            glUniformMatrix4fv(glGetUniformLocation(shader->id, "uprojection"), 1, false, &proj[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shader->id, "uview"), 1, false, &view[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shader->id, "umodel"), 1, false, model);

            glDrawElements(GL_TRIANGLES, call->mesh.mesh->n_indices, GL_UNSIGNED_INT, 0);

            break;
        }
        case text_drawcall: {
            glUseProgram(text_shader.id);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, call->text.font->texture_id);

            glUniform1i(glGetUniformLocation(text_shader.id, "utexture"), 0);

            glBindVertexArray(text_mash.VAO);
            mat4 proj;
            glm_ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f, proj);
            glUniformMatrix4fv(glGetUniformLocation(text_shader.id, "projection"), 1, GL_FALSE, &proj[0][0]);
            glBindBuffer(GL_ARRAY_BUFFER, text_mash.VBO);
            n_text_vertices = 0;

            float x = call->text.x;
            float y = call->text.y;

            for (size_t i = 0; i < call->text.content_len; i++) {
               unsigned char c = call->text.content[i];
                if (c < 32 || c > 127) { c = 127; } // del

                font_face *font = call->text.font;
                glyph_metrics *glyph = &font->glyphs[c - 32];

                int width = glyph->width;
                int height = glyph->height;

                vec2 positions[4];
                glm_vec2_scale (
                    (vec2){0.0f, (float)glyph->bearingy},
                    call->text.scale,
                    positions[0]
                );
                glm_vec2_scale(
                    (vec2){0.0f, -(float)height+(float)glyph->bearingy},
                    call->text.scale,
                    positions[1]
                );
                glm_vec2_scale(
                    (vec2){(float)width, -(float)height+(float)glyph->bearingy},
                    call->text.scale,
                    positions[2]
                );
                glm_vec2_scale(
                    (vec2){(float)width, (float)glyph->bearingy},
                    call->text.scale,
                    positions[3]
                );
                vec2 uv_min = {
                    (float)glyph->texture_x / (float)font->atlas_width,
                    (float)glyph->texture_y / (float)font->atlas_height
                };
                vec2 uv_max = {
                    (float)(glyph->texture_x + width) / (float)font->atlas_width,
                    (float)(glyph->texture_y + height) / (float)font->atlas_height
                };

                text_data_buffer[n_text_vertices++] = (text_data){
                    .pos = {positions[0][0] + x, positions[0][1] + y},
                    .uv = {uv_min[0], uv_min[1]},
                    .colour = {
                        call->text.colour[0],
                        call->text.colour[1],
                        call->text.colour[2],
                        call->text.colour[3],
                    },
                };
                text_data_buffer[n_text_vertices++] = (text_data){
                    .pos = {positions[1][0] + x, positions[1][1] + y},
                    .uv = {uv_min[0], uv_max[1]},
                    .colour = {
                        call->text.colour[0],
                        call->text.colour[1],
                        call->text.colour[2],
                        call->text.colour[3],
                    },
                };
                text_data_buffer[n_text_vertices++] = (text_data){
                    .pos = {positions[2][0] + x, positions[2][1] + y},
                    .uv = {uv_max[0], uv_max[1]},
                    .colour = {
                        call->text.colour[0],
                        call->text.colour[1],
                        call->text.colour[2],
                        call->text.colour[3],
                    },
                };
                text_data_buffer[n_text_vertices++] = (text_data){
                    .pos = {positions[2][0] + x, positions[2][1] + y},
                    .uv = {uv_max[0], uv_max[1]},
                    .colour = {
                        call->text.colour[0],
                        call->text.colour[1],
                        call->text.colour[2],
                        call->text.colour[3],
                    },
                };
                text_data_buffer[n_text_vertices++] = (text_data){
                    .pos = {positions[3][0] + x, positions[3][1] + y},
                    .uv = {uv_max[0], uv_min[1]},
                    .colour = {
                        call->text.colour[0],
                        call->text.colour[1],
                        call->text.colour[2],
                        call->text.colour[3],
                    },
                };
                text_data_buffer[n_text_vertices++] = (text_data){
                    .pos = {positions[0][0] + x, positions[0][1] + y},
                    .uv = {uv_min[0], uv_min[1]},
                    .colour = {
                        call->text.colour[0],
                        call->text.colour[1],
                        call->text.colour[2],
                        call->text.colour[3],
                    },
                };

                x += (float)glyph->advance * call->text.scale;
            }
            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(text_data) * n_text_vertices,
                text_data_buffer,
                GL_DYNAMIC_DRAW);

            glDepthMask(GL_FALSE);
            glDrawArrays(GL_TRIANGLES, 0, n_text_vertices);
            glDepthMask(GL_TRUE);

            break;
        }
    }
}

void process_calls(draw_call *calls, size_t n_calls, view_proj *cam_matrices, size_t n_cams) {
    for (size_t i = 0; i < n_cams; i ++) {
        view_proj *vp = &cam_matrices[i];
        for (size_t j = 0; j < n_calls; j ++) {
            run_draw_call(&calls[j], vp->view, vp->proj);
        }
    }
}
