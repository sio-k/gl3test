#pragma once
/*
 * Copyright (C) 2017 Sio Kreuzer. All Rights Reserved.
 */

// simple abstraction for loading/compiling shaders and working with them

#include "libs.h"

#include "renderer_data.h"

typedef struct Shader
{
    GLuint num;
} Shader;

void shader(
    Shader* sh,
    GLenum shader_type,
    const char* path_to_source_file,
    GLuint shader_program
);
void shaderDelete( Shader* sh );
void shaderDetach( Shader* sh, GLuint shader_program );

typedef struct ShaderProgram
{
    GLuint prog;
    Shader vert;
    Shader tesc;
    Shader tese;
    Shader geom;
    Shader frag;
    Shader compute;
} ShaderProgram;

void shaderProgram(
    ShaderProgram* sh,
    const char* vert_src, // path to vertex shader source file
    const char* tesc_src, // path to tesselation control shader source file
    const char* tese_src, // path to tesselation evaluation shader source file
    const char* geom_src, // path to geometry shader source file
    const char* frag_src, // path to fragment shader source file
    const char*
    compute_src  // path to compute shader source file (must be NULL if any other arguments except sh are non-NULL)
    // NOTE: compute_src UNUSED as of now, need to implement some better ways of custom attribute binding to allow for compute shaders to be properly usable (and different shader pipelines)
);
void computeShader(
    ShaderProgram* sh,
    const char* prog_src
);
void shaderProgramDestroy( ShaderProgram* sh );

void useShaderProgram( ShaderProgram* sh );
