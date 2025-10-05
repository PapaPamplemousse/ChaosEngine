#include "chaos_shader.h"
#include "chaos_linalg.h"

#include <glad/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------------------- */
/* Built-in unlit shaders (used by chaos_draw_mesh and debug helpers)         */
/* -------------------------------------------------------------------------- */

/* Internal helper: compile+link GLSL program */
t_uint32 gl_make_program(const char* vs_src, const char* fs_src)
{
    t_uint32 vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_src, NULL);
    glCompileShader(vs);

    t_uint32 fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_src, NULL);
    glCompileShader(fs);

    t_uint32 prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}

const char* VS_UNLIT_COLOR =
    "#version 450 core\n"
    "layout(std140, binding=0) uniform UBO { mat4 uView; mat4 uProj; };\n"
    "layout(location=0) in vec3 aPos;\n"
    "layout(location=1) in vec3 aNormal;\n"
    "layout(location=2) in vec2 aUV;\n"
    "uniform mat4 uModel;\n"
    "void main(){ gl_Position = uProj * uView * uModel * vec4(aPos,1.0); }\n";

const char* FS_UNLIT_COLOR =
    "#version 450 core\n"
    "layout(location=0) out vec4 oColor;\n"
    "uniform vec4 uColor;\n"
    "void main(){ oColor = uColor; }\n";

const char* VS_UNLIT_TEX =
    "#version 450 core\n"
    "layout(std140, binding=0) uniform UBO { mat4 uView; mat4 uProj; };\n"
    "layout(location=0) in vec3 aPos;\n"
    "layout(location=1) in vec3 aNormal;\n"
    "layout(location=2) in vec2 aUV;\n"
    "uniform mat4 uModel;\n"
    "out vec2 vUV;\n"
    "void main(){ vUV=aUV; gl_Position = uProj * uView * uModel * vec4(aPos,1.0); }\n";

const char* FS_UNLIT_TEX =
    "#version 450 core\n"
    "layout(location=0) out vec4 oColor;\n"
    "in vec2 vUV;\n"
    "uniform sampler2D uTex;\n"
    "uniform vec4 uColor;\n"
    "void main(){ oColor = texture(uTex, vUV) * uColor; }\n";




/* -------------------------------------------------------------------------- */
/* Shader helpers                                                              */
/* -------------------------------------------------------------------------- */

chaos_shader_t chaos_shader_create(const char* vs_src, const char* fs_src)
{
    chaos_shader_t s = {0};
    s._gl_prog = gl_make_program(vs_src, fs_src);
    return s;
}

void chaos_shader_destroy(chaos_shader_t* s)
{
    if (s && s->_gl_prog) glDeleteProgram(s->_gl_prog);
    if (s) s->_gl_prog = 0;
}

t_sint32 chaos_shader_uniform(const chaos_shader_t* s, const char* name)
{
    if (!s || !s->_gl_prog || !name) return -1;
    return (t_sint32)glGetUniformLocation(s->_gl_prog, name);
}

void chaos_shader_set_mat4(t_sint32 loc, const mat4* M)
{
    if (loc < 0 || !M) return;
    t_float32 Mf[16]; m4_to_f32(M, Mf);
    glUniformMatrix4fv((GLint)loc, 1, GL_FALSE, Mf);
}

void chaos_shader_set_vec4(t_sint32 loc, const t_float32 rgba[4])
{
    if (loc < 0 || !rgba) return;
    glUniform4fv((GLint)loc, 1, rgba);
}

void chaos_shader_bind(const chaos_shader_t* shader)
{
    if (!shader) return;
    glUseProgram(shader->_gl_prog);
}

void chaos_shader_set_f32(t_sint32 loc, t_float32 v)
{
    if (loc < 0) return;
    glUniform1f((GLint)loc, v);
}
