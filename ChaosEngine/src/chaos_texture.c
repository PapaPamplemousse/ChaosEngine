#include "chaos_texture.h"

#include <glad/gl.h>
#include <stdlib.h>
#include <string.h>


chaos_texture_t chaos_texture_create(const t_uint8* pixels, t_uint32 w, t_uint32 h, t_uint32 ch)
{
    chaos_texture_t t = {0}; t.width = w; t.height = h; t.channels = ch;
    glGenTextures(1, &t._priv_gl_tex);
    glBindTexture(GL_TEXTURE_2D, t._priv_gl_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
    const GLenum fmt = (ch == 4) ? GL_RGBA : GL_RGB;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, fmt, (GLsizei)w, (GLsizei)h, 0, fmt, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    return t;
}

void chaos_texture_destroy(chaos_texture_t* t)
{
    if (t && t->_priv_gl_tex) glDeleteTextures(1, &t->_priv_gl_tex);
    memset(t, 0, sizeof *t);
}