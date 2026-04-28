#include "render.h"
#include "memory.h"
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture *GenTextureFromFileName(const char *filename) {
    Texture *result = AllocMem(sizeof(Texture));

    stbi_set_flip_vertically_on_load(true);
    const unsigned char *data = stbi_load(
        filename,
        &result->width,
        &result->height,
        &result->nChannels,
        0
    );
    result->data = data;

    glGenTextures(1, &result->ID); GL_CHECK()

    return result;
}
void UploadTexture(Texture *texture) {
    GLenum gl_texture_type;
    GLenum gl_min_filter;
    GLenum gl_mag_filter;
    GLenum gl_format = texture->nChannels == 4 ? GL_RGBA : GL_RGB;

    switch (texture->type) {
        case TEXTURE_TYPE_CUBE_MAP: { gl_texture_type = GL_TEXTURE_CUBE_MAP; break; }
        case TEXTURE_TYPE_3D: { gl_texture_type = GL_TEXTURE_3D; break; }
        case TEXTURE_TYPE_2D_ARRAY: { gl_texture_type = GL_TEXTURE_2D_ARRAY; break; }
        default: { gl_texture_type = GL_TEXTURE_2D; break; }
    }

    switch (texture->min_filter) {
        case TEXTURE_FILTER_NEAREST: { gl_min_filter = GL_NEAREST; break; }
        case TEXTURE_FILTER_MIPMAP_NEAREST: { gl_min_filter = GL_NEAREST_MIPMAP_NEAREST; break; }
        case TEXTURE_FILTER_MIPMAP_LINEAR: { gl_min_filter = GL_LINEAR_MIPMAP_LINEAR; break; }
        default: { gl_min_filter = GL_LINEAR; break; }
    }
    
    switch (texture->mag_filter) {
        case TEXTURE_FILTER_NEAREST: gl_mag_filter = GL_NEAREST; break;
        case TEXTURE_FILTER_MIPMAP_NEAREST: gl_mag_filter = GL_NEAREST_MIPMAP_NEAREST; break;
        case TEXTURE_FILTER_MIPMAP_LINEAR: gl_mag_filter = GL_LINEAR_MIPMAP_LINEAR; break;
        default: gl_mag_filter = GL_LINEAR; break;
    }

    glActiveTexture(GL_TEXTURE0); GL_CHECK()
    glBindTexture(gl_texture_type, texture->ID); GL_CHECK()
    glTexParameteri(gl_texture_type, GL_TEXTURE_MIN_FILTER, gl_min_filter); GL_CHECK()
    glTexParameteri(gl_texture_type, GL_TEXTURE_MAG_FILTER, gl_mag_filter); GL_CHECK()
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        gl_texture_type, 
        0, gl_format,
        texture->width, texture->height,
        0, gl_format,
        GL_UNSIGNED_BYTE, texture->data
    ); GL_CHECK()
}