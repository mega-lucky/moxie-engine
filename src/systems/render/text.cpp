#include <ft2build.h>
#include FT_FREETYPE_H
#include <text.h>
#include <glad/glad.h>
#include <cstdio>

font_face *load_font_from_file(const char *path, unsigned size) {
    FT_Library ft;
    FT_Init_FreeType(&ft);
    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face)) {
        abort();
    };

    FT_Set_Pixel_Sizes(face, 0, size);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    font_face *result = new font_face;

    int atlas_width = 0, atlas_height = 0;
    for (unsigned char c = 32; c < 128; c ++) {
        FT_Load_Glyph(face, FT_Get_Char_Index(face, c), FT_LOAD_RENDER);
        
        auto *glyph = &result->glyphs[c - 32];
        
        glyph->advance  = face->glyph->metrics.horiAdvance >> 6;
        glyph->bearingx = face->glyph->metrics.horiBearingX >> 6;
        glyph->bearingy = face->glyph->metrics.horiBearingY >> 6;
        glyph->width    = face->glyph->metrics.width >> 6;
        glyph->height   = face->glyph->metrics.height >> 6;

        atlas_width += face->glyph->bitmap.width;
        atlas_height = std::max(atlas_height, glyph->height);
    }

    int x = 0;
    auto *buffer = new unsigned char[atlas_height * atlas_width]();

    for (unsigned char c = 32; c < 128; c ++) {
        FT_Load_Char(face, c, FT_LOAD_RENDER);
        
        
        int width = face->glyph->bitmap.width;
        for (unsigned row = 0; row < face->glyph->bitmap.rows; row ++) {
            std::copy(
                face->glyph->bitmap.buffer + row * width,
                face->glyph->bitmap.buffer + row * width + width,
                buffer + x + row * atlas_width
            );
        }
        
        auto *glyph = &result->glyphs[c - 32];
        glyph->texture_x = x;
        glyph->texture_y = 0;
        x += face->glyph->bitmap.width;
    }
    
    GLuint textureid;
    glGenTextures(1, &textureid);
    glBindTexture(GL_TEXTURE_2D, textureid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    result->texture_id = textureid;
    result->atlas_width = atlas_width;
    result->atlas_height = atlas_height;
    result->line_height = 0;

    delete[] buffer;
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return result;
}