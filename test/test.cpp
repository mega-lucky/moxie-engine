#include <engine.hpp>
#include <render_system.hpp>
#include <cglm/quat.h>

#include <iostream>

GLuint gen_checkers(uint8_t c1[3], uint8_t c2[3]) {
    uint8_t buffer[2 * 2 * 3] = {
        c2[0], c2[1], c2[2], c1[0], c1[1], c1[2],
        c1[0], c1[1], c1[2], c2[0], c2[1], c2[2],
    };

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texture_id;
}

int main() {
    Engine new_engine;
    World::Registry &world = new_engine.WorldRegistry;
    new_engine.MainWindow.Show();

    world.RegisterComponent<mesh_renderer>();
    world.RegisterComponent<mesh_shape>();
    world.RegisterComponent<transform>();

    material_data material;
    shader_data shader;
    texture_data texture;
    texture_data *texptr;

    uint8_t c1[] = {0,0,255};
    uint8_t c2[] = {0,255,0};

    texture.id = gen_checkers(c1, c2);
    texture.type = ALBEDO_TEXTURE;
    texptr = &texture;

    init_shader_from_file(&shader, "assets/shaders/textured_vert.glsl", "assets/shaders/textured_frag.glsl");
    material.shader = &shader;
    material.n_textures = 1;
    material.textures = &texptr;

    mesh_shape mesh;

    mesh_vertex vertex_data[] = {
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {255, 255, 255, 255}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {255, 255, 255, 255}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {255, 255, 255, 255}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {255, 255, 255, 255}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {255, 255, 255, 255}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, {255, 255, 255, 255}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {255, 255, 255, 255}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {255, 255, 255, 255}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {255, 255, 255, 255}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {255, 255, 255, 255}},
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {255, 255, 255, 255}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {255, 255, 255, 255}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {255, 255, 255, 255}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {255, 255, 255, 255}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {255, 255, 255, 255}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {255, 255, 255, 255}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {255, 255, 255, 255}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {255, 255, 255, 255}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {255, 255, 255, 255}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {255, 255, 255, 255}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}, {255, 255, 255, 255}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}, {255, 255, 255, 255}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {255, 255, 255, 255}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, {255, 255, 255, 255}}
    };
    unsigned int index_data[] = {
        0, 2, 3,
        0, 3, 1,
        4, 5, 7,
        6, 4, 7,
        8, 10, 9,
        9, 10, 11,
        12, 14, 13,
        13, 14, 15,
        16, 18, 17,
        17, 18, 19,
        20, 22, 21,
        21, 22, 23
    };

    init_mesh(&mesh, vertex_data, 24, index_data, 36);
    upload_mesh(&mesh);

    Entity a = world.NewEntity();
    world.GiveComponent<mesh_shape>(a, mesh);
    world.GiveComponent<transform>(a, (transform){
        .position = {-1.0f,0.0f,-5.0f},
        .scale = {2.0f,2.0f,2.0f},
        .rotation = {0.0f,0.0f,0.0f,1.0f},
    });
    world.GiveComponent<mesh_renderer>(a, (mesh_renderer){
        .material = &material,
    });

    Entity b = world.NewEntity();
    world.GiveComponent<mesh_shape>(b, mesh);
    world.GiveComponent<transform>(b, (transform){
        .position = {1.0f,0.0f,-5.0f},
        .scale = {1.0f,1.0f,1.0f},
        .rotation = {0.0f,0.0f,0.0f,1.0f},
    });
    world.GiveComponent<mesh_renderer>(b, (mesh_renderer){
        .material = &material,
    });

    world.RegisterSystem<RenderSystem>(new_engine);

    while (!new_engine.MainWindow.ShouldClose()) {
        std::cout << new_engine.Timer.GetFPS() << std::endl;
        transform &t = world.GetComponent<transform>(a);
        transform &t2 = world.GetComponent<transform>(b);
        glm_quat(t.rotation, new_engine.Timer.GetTime(), 1.0f, 1.0f, 0.0f);
        glm_quat(t2.rotation, new_engine.Timer.GetTime(), 1.0f, 0.0f, 0.0f);
        new_engine.Update();
    }

    destroy_mesh(&mesh);
}