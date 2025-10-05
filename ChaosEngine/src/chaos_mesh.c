#include "chaos_mesh.h"
#include "chaos_linalg.h"

#include <glad/gl.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <stdio.h>


chaos_mesh_t chaos_mesh_create(
    const t_float32* positions,
    const t_float32* normals,
    const t_float32* uvs,
    t_uint32 vertex_count,
    const t_uint32* indices,
    t_uint32 index_count)
{
    chaos_mesh_t m = {0};
    m.vertex_count = vertex_count;
    m.index_count  = index_count;

    GLuint vao = 0, vbo = 0, ibo = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    if (positions) {
        // Interleaved layout (pos + normal + uv)
        size_t stride = 8 * sizeof(float);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * stride, positions, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    }

    if (indices) {
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(t_uint32), indices, GL_STATIC_DRAW);
    }

    glBindVertexArray(0);

    m._priv_gl_vao = vao;
    m._priv_gl_vbo = vbo;   // ✅ ton struct a ce champ, pas "_vbo_pos"
    m._priv_gl_ibo = ibo;

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        printf("⚠️ chaos_mesh_create() GL error: 0x%x\n", err);

    return m;
}


void chaos_mesh_destroy(chaos_mesh_t* m)
{
    if (!m) return;
    if (m->_priv_gl_ibo) glDeleteBuffers(1, &m->_priv_gl_ibo);
    if (m->_priv_gl_vbo) glDeleteBuffers(1, &m->_priv_gl_vbo);
    if (m->_priv_gl_vao) glDeleteVertexArrays(1, &m->_priv_gl_vao);
    memset(m, 0, sizeof *m);
}

chaos_mesh_t chaos_mesh_create_sphere(t_float32 radius, t_uint32 slices, t_uint32 stacks)
{
    const t_uint32 vcount = (stacks + 1) * (slices + 1);
    const t_uint32 icount = stacks * slices * 6;

    t_float32* positions = malloc(vcount * 3 * sizeof(t_float32));
    t_uint32*  indices   = malloc(icount * sizeof(t_uint32));

    t_uint32 vi = 0;
    for (t_uint32 i = 0; i <= stacks; ++i) {
        t_float64 v = (t_float64)i / (t_float64)stacks;
        t_float64 phi = v * C_PI;
        for (t_uint32 j = 0; j <= slices; ++j) {
            t_float64 u = (t_float64)j / (t_float64)slices;
            t_float64 theta = u * 2.0 * C_PI;
            positions[vi++] = (t_float32)(radius * chaos_sin(phi) * chaos_cos(theta));
            positions[vi++] = (t_float32)(radius * chaos_cos(phi));
            positions[vi++] = (t_float32)(radius * chaos_sin(phi) * chaos_sin(theta));
        }
    }

    t_uint32 ii = 0;
    for (t_uint32 i = 0; i < stacks; ++i) {
        for (t_uint32 j = 0; j < slices; ++j) {
            t_uint32 a = i * (slices + 1) + j;
            t_uint32 b = a + slices + 1;
            indices[ii++] = a;
            indices[ii++] = b;
            indices[ii++] = a + 1;
            indices[ii++] = b;
            indices[ii++] = b + 1;
            indices[ii++] = a + 1;
        }
    }

    chaos_mesh_t sphere = chaos_mesh_create(positions, NULL, NULL, vcount, indices, icount);

    free(positions);
    free(indices);
    return sphere;
}


chaos_mesh_t chaos_mesh_create_cube(t_float32 size)
{
    const t_float32 s = size * 0.5f;
    const t_float32 pos[] = {
        -s,-s,-s,  s,-s,-s,  s, s,-s, -s, s,-s,
        -s,-s, s,  s,-s, s,  s, s, s, -s, s, s
    };
    const uint32_t idx[] = {
        0,1,2, 0,2,3,   // back
        4,5,6, 4,6,7,   // front
        0,4,7, 0,7,3,   // left
        1,5,6, 1,6,2,   // right
        3,2,6, 3,6,7,   // top
        0,1,5, 0,5,4    // bottom
    };
    return chaos_mesh_create(pos, NULL, NULL, 8, idx, 36);
}
