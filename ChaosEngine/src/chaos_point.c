#include "chaos_points.h"
#include "chaos_shader.h"
#include "chaos_linalg.h"

#include <glad/gl.h>
#include <stdlib.h>


/* -------------------------------------------------------------------------- */
/* Points stream API (for particle systems rendered as GL_POINTS)             */
/* -------------------------------------------------------------------------- */

chaos_points_t chaos_points_create(t_uint32 capacity)
{
    chaos_points_t ps = {0};
    ps.capacity = capacity;

    glGenVertexArrays(1, &ps._gl_vao);
    glGenBuffers(1, &ps._gl_vbo);

    glBindVertexArray(ps._gl_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ps._gl_vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(capacity * (sizeof(t_float32)*(3+1+1))), NULL, GL_STREAM_DRAW);

    const GLsizei stride = (GLsizei)(sizeof(t_float32)*(3 + 1 + 1));
    const uintptr_t off_pos   = 0;
    const uintptr_t off_size  = sizeof(t_float32)*3;
    const uintptr_t off_alpha = sizeof(t_float32)*4;
    glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)off_pos);
    glEnableVertexAttribArray(1); glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride, (const void*)off_size);
    glEnableVertexAttribArray(2); glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (const void*)off_alpha);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return ps;
}

void chaos_points_destroy(chaos_points_t* ps)
{
    if (!ps) return;
    if (ps->_gl_vbo) glDeleteBuffers(1, &ps->_gl_vbo);
    if (ps->_gl_vao) glDeleteVertexArrays(1, &ps->_gl_vao);
    memset(ps, 0, sizeof *ps);
}

void chaos_points_update(const chaos_points_t* ps, const void* data, t_uint32 count, t_uint32 stride_bytes)
{
    if (!ps || !ps->_gl_vbo || !data || count == 0) return;
    const GLsizeiptr size = (GLsizeiptr)((t_uint64)count * (t_uint64)stride_bytes);
    glBindBuffer(GL_ARRAY_BUFFER, ps->_gl_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void chaos_points_draw(const chaos_points_t* ps, const chaos_shader_t* shader, t_uint32 count)
{
    if (!ps || !shader || !shader->_gl_prog || count == 0) return;

    glUseProgram(shader->_gl_prog);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glBindVertexArray(ps->_gl_vao);
    glDrawArrays(GL_POINTS, 0, (GLsizei)count);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
}

