#include "./types.h"
#include "./mesh.h"
#include <algorithm>

void xinit_mesh(
    mesh_shape *data,
    const void *vert_data, size_t nverts,
    const unsigned int *ind_data, size_t ninds,
    const attrib_layout *layout
) {
    data->layout.n_attribs = layout->n_attribs;
    data->layout.attribs = new vert_attrib_data[layout->n_attribs];
    std::copy(layout->attribs, layout->attribs + layout->n_attribs, data->layout.attribs);
    data->layout.stride = layout->stride;

    data->vertex_data = ::operator new(nverts * layout->stride);

    const char *src_begin = static_cast<const char*>(vert_data);
    const char *src_end = src_begin + nverts * layout->stride;
    char *dest = static_cast<char *>(data->vertex_data);

    std::copy(src_begin, src_end, dest);
    data->n_vertices = nverts;

    data->index_data = new unsigned int[ninds];
    std::copy(ind_data, ind_data + ninds, data->index_data);
    data->n_indices = ninds;

    glGenVertexArrays(1, &data->VAO);
    glGenBuffers(1, &data->VBO);
    glGenBuffers(1, &data->EBO);
}

void init_mesh(
    mesh_shape *data,
    const mesh_vertex *vert_data, size_t nverts,
    const unsigned int *ind_data, size_t ninds
) {
    vert_attrib_data text_attribs[4] = {
        {.size=3,.type=ATTRIB_F32,.normalised=false,.offset=offsetof(mesh_vertex, position)},
        {.size=3,.type=ATTRIB_F32,.normalised=false,.offset=offsetof(mesh_vertex, normal)},
        {.size=2,.type=ATTRIB_F32,.normalised=false,.offset=offsetof(mesh_vertex, uv)},
        {.size=4,.type=ATTRIB_U8,.normalised=true,.offset=offsetof(mesh_vertex, colour)}
    };
    attrib_layout layout = {
        .attribs = text_attribs,
        .n_attribs = 4,
        .stride = sizeof(mesh_vertex)
    };

    xinit_mesh(data, vert_data, nverts, ind_data, ninds, &layout);
}

static inline GLenum get_opengl_type(enum attrib_type type) {
    switch (type) {
        case ATTRIB_F32: return GL_FLOAT;
        case ATTRIB_U8: return GL_UNSIGNED_BYTE;
        default: return 0;
    }
}

static inline size_t get_attrib_bytes(const vert_attrib_data &attrib) {
    size_t size = attrib.size;
    switch (attrib.type) {
        case ATTRIB_F32: size *= sizeof(GLfloat); break;
        case ATTRIB_U8: size *= sizeof(GLchar); break;
    }
    return size;
}

void upload_mesh(const mesh_shape *mesh) {
    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        mesh->n_vertices * mesh->layout.stride,
        mesh->vertex_data,
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        mesh->n_indices * sizeof(unsigned int),
        mesh->index_data,
        GL_STATIC_DRAW
    );

    for (size_t i = 0; i < mesh->layout.n_attribs; i ++) {
        vert_attrib_data &attrib = mesh->layout.attribs[i];

        glVertexAttribPointer(i,
            attrib.size,
            get_opengl_type(attrib.type),
            attrib.normalised,
            mesh->layout.stride,
            (void*)attrib.offset
        );
        glEnableVertexAttribArray(i);
    }
}

void destroy_mesh(mesh_shape *data) {
    ::operator delete[](data->vertex_data);
    data->vertex_data = nullptr;
    data->n_vertices = 0;

    delete[] data->index_data;
    data->index_data = nullptr;
    data->n_indices = 0;

    data->layout = {0};

    glDeleteVertexArrays(1, &data->VAO);
    glDeleteBuffers(1, &data->VBO);
    glDeleteBuffers(1, &data->EBO);
}