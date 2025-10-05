#include "chaos_draw.h"
#include "chaos_shader.h"
#include "chaos_mesh.h"
#include "chaos_texture.h"
#include "chaos_camera.h"
#include "chaos_linalg.h"

#include <glad/gl.h>
#include <string.h>

#include <stdio.h>

/* -------------------------------------------------------------------------- */
/* Draw calls                                                                  */
/* -------------------------------------------------------------------------- */


/* ************* STATIC PROTOTYPES ************* */
static void set_model_uniform(GLuint prog, const mat4* model);


/* ************* FUNCTIONS ************* */
void chaos_draw_mesh(chaos_renderer_t* r,
                     const chaos_mesh_t* m,
                     const chaos_material_t* mat,
                     const mat4* model)
{
    if (!m || !mat) return;

    /* Sélection du shader */
    const GLuint prog = (mat->type == CHAOS_MAT_UNLIT_TEXTURE)
                            ? r->_gl_prog_unlit_tex
                            : r->_gl_prog_unlit_color;
    glUseProgram(prog);

    if (!glIsProgram(prog)) printf("❌ Program %u invalide\n", prog);

    set_model_uniform(prog, model);

    const GLint locColor = glGetUniformLocation(prog, "uColor");
    if (locColor >= 0)
        glUniform4fv(locColor, 1, mat->color);

    if (mat->type == CHAOS_MAT_UNLIT_TEXTURE && mat->tex && mat->tex->_priv_gl_tex) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mat->tex->_priv_gl_tex);
        const GLint locTex = glGetUniformLocation(prog, "uTex");
        if (locTex >= 0) glUniform1i(locTex, 0);
    }


    if (m->_priv_gl_ibo && m->index_count) {
        GLenum errCheck = glGetError();
        if (errCheck != GL_NO_ERROR)
            printf("⚠️ gl pre-draw error: 0x%x\n", errCheck);
        glDrawElements(GL_TRIANGLES, (GLsizei)m->index_count, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m->vertex_count);
    }

    glBindVertexArray(0);

    if (mat->type == CHAOS_MAT_UNLIT_TEXTURE)
        glBindTexture(GL_TEXTURE_2D, 0);
}


void chaos_debug_draw_grid(chaos_renderer_t* r, t_float32 half_x, t_float32 half_z, t_float32 step, t_float32 y)
{
    const t_uint32 lines_x = (t_uint32)((half_x*2)/step) + 1;
    const t_uint32 lines_z = (t_uint32)((half_z*2)/step) + 1;
    const t_uint32 V = (lines_x + lines_z) * 2;

    t_float32* buf = (t_float32*)malloc(sizeof(t_float32)*V*3);
    t_uint32 k=0;
    for (t_float32 x=-half_x; x<=half_x+1e-4f; x+=step){
        buf[k++]=x; buf[k++]=y; buf[k++]=-half_z;  buf[k++]=x; buf[k++]=y; buf[k++]= half_z;
    }
    for (t_float32 z=-half_z; z<=half_z+1e-4f; z+=step){
        buf[k++]=-half_x; buf[k++]=y; buf[k++]=z;  buf[k++]= half_x; buf[k++]=y; buf[k++]=z;
    }

    GLuint vao,vbo; glGenVertexArrays(1,&vao); glGenBuffers(1,&vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(t_float32)*V*3, buf, GL_STREAM_DRAW);
    glEnableVertexAttribArray(0); glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(t_float32),(void*)0);

    glUseProgram(r->_gl_prog_unlit_color);
    mat4 I = m4_identity(); set_model_uniform(r->_gl_prog_unlit_color, &I);
    const t_float32 color[4] = {0.3f,0.3f,0.35f,1.0f};
    const GLint locC = glGetUniformLocation(r->_gl_prog_unlit_color, "uColor"); if (locC>=0) glUniform4fv(locC,1,color);
    glDrawArrays(GL_LINES, 0, (GLsizei)V);

    glBindBuffer(GL_ARRAY_BUFFER,0); glBindVertexArray(0);
    glDeleteBuffers(1,&vbo); glDeleteVertexArrays(1,&vao);
    free(buf);
}

void chaos_debug_draw_line3(chaos_renderer_t* r, vec_t a, vec_t b, t_float32 rgba[4])
{
    const t_float32 buf[6] = {
        (t_float32)a.x,(t_float32)a.y,(t_float32)a.z,
        (t_float32)b.x,(t_float32)b.y,(t_float32)b.z
    };
    GLuint vao,vbo; glGenVertexArrays(1,&vao); glGenBuffers(1,&vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof buf, buf, GL_STREAM_DRAW);
    glEnableVertexAttribArray(0); glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(t_float32),(void*)0);
    glUseProgram(r->_gl_prog_unlit_color);
    mat4 I = m4_identity(); set_model_uniform(r->_gl_prog_unlit_color, &I);
    const GLint locC = glGetUniformLocation(r->_gl_prog_unlit_color, "uColor"); if (locC>=0) glUniform4fv(locC,1,rgba);
    glDrawArrays(GL_LINES, 0, 2);
    glBindBuffer(GL_ARRAY_BUFFER,0); glBindVertexArray(0);
    glDeleteBuffers(1,&vbo); glDeleteVertexArrays(1,&vao);
}

/* 2D sprite helper kept unchanged (uses temporary quad + ortho override) */
void chaos_draw_sprite2D(chaos_renderer_t* r, const chaos_texture_t* tex, t_float32 x, t_float32 y, t_float32 w, t_float32 h, t_float32 rgba[4])
{
    t_float32 verts[] = {
        /* pos        normal  uv */
         0, 0, 0,     0,0,1,  0,0,
         w, 0, 0,     0,0,1,  1,0,
         w, h, 0,     0,0,1,  1,1,
         0, h, 0,     0,0,1,  0,1,
    };
    t_uint32 idx[] = {0,1,2, 0,2,3};
    
    glUseProgram(r->_gl_prog_unlit_color); // ou _unlit_tex si texture
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        printf("⚠️ glUseProgram error: 0x%x\n", err);


    chaos_mesh_t quad = chaos_mesh_create(verts, verts+3, verts+6, 4, idx, 6);

    printf("[DEBUG] Created mesh: vao=%u vbo=%u ibo=%u\n",
       quad._priv_gl_vao, quad._priv_gl_vbo, quad._priv_gl_ibo);

    const mat4 savedV = r->camera.view, savedP = r->camera.proj;

    mat4 V = m4_identity();
    mat4 P = m4_identity();

    P.m[0]  =  2.0f / (t_float32)r->width;
    P.m[5]  =  2.0f / (t_float32)r->height;
    P.m[10] = -1.0f;
    P.m[12] = -1.0f;  /* translation X */
    P.m[13] = -1.0f;  /* translation Y */


    r->camera.view = V; r->camera.proj = P; chaos_upload_vp_ubo(r);

    chaos_material_t m = {0};
    m.type = CHAOS_MAT_UNLIT_COLOR; m.tex = NULL;
    m.color[0]=rgba[0]; m.color[1]=rgba[1]; m.color[2]=rgba[2]; m.color[3]=rgba[3];

    mat4 M = m4_identity(); 
    M.m[12] = x; 
    M.m[13] = y; 
    M.m[14] = -1.0f;

    

    chaos_draw_mesh(r, &quad, &m, &M);

    r->camera.view = savedV; r->camera.proj = savedP; chaos_upload_vp_ubo(r);
    chaos_mesh_destroy(&quad);
}


static void set_model_uniform(GLuint prog, const mat4* model)
{
    const GLint loc = glGetUniformLocation(prog, "uModel");
    if (loc >= 0 && model) {
        t_float32 M[16]; m4_to_f32(model, M);
        glUniformMatrix4fv(loc, 1, GL_FALSE, M);
    }
}