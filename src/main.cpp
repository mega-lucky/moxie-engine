#include <engine.hpp>
#include <render_system.hpp>
#include <cglm/quat.h>
#include <iostream>
#include <lua.h>
#include <lualib.h>

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

    ComponentID Transform = world.RegisterComponent<transform>();
    ComponentID MeshShape = world.RegisterComponent<mesh_shape>();
    ComponentID MeshRender = world.RegisterComponent<mesh_renderer>();
    world.StoreComponentID(Transform, "Transform");
    world.StoreComponentID(MeshShape, "MeshShape");
    world.StoreComponentID(MeshRender, "MeshRender");

    world.RegisterSystem<RenderSystem>(new_engine);

    material_data material;
    shader_data shader;
    texture_data texture;
    texture_data *texptr;

    uint8_t c1[] = {0,0,0};
    uint8_t c2[] = {255,0,255};

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

    new_engine.Assets.meshes.insert({"Cube", &mesh});
    new_engine.Assets.materials.insert({"Checkers", &material});
    
    new_engine.ScriptSchuduler.LoadFile("test/test.luau");

    while (!new_engine.MainWindow.ShouldClose()) {
        new_engine.Update();
    }

    destroy_mesh(&mesh);
}
