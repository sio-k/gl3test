#pragma once
/*
 * Copyright (C) 2017 Sio Kreuzer. All Rights Reserved.
 */

#include "libs.h"

/*
 * Data you feed the renderer and data the renderer gives you back.
 * Mainly this is gonna be some basic structs for convenience and a bunch
 * of typedefs.
 */

// vectors
// @{

typedef f32 vec2[2];
enum
{
    VEC2_X = 0,
    VEC2_Y = 1,
    VEC2_S = 0,
    VEC2_T = 1,
    VEC2_LENGTH = 2,
    VEC2_SIZE = 2 * sizeof( f32 )
};

typedef f32 vec3[3];
enum
{
    VEC3_X = 0,
    VEC3_Y = 1,
    VEC3_Z = 2,
    VEC3_S = 0,
    VEC3_T = 1,
    VEC3_U = 2,
    VEC3_LENGTH = 3,
    VEC3_SIZE = 3 * sizeof( f32 )
};

typedef f32 vec4[4];
enum
{
    VEC4_X = 0,
    VEC4_Y = 1,
    VEC4_Z = 2,
    VEC4_W = 3,
    VEC4_LENGTH = 4,
    VEC4_SIZE = 4 * sizeof( f32 )
};

typedef vec4 color_vec;
enum
{
    COLOR_VEC_R = VEC4_X,
    COLOR_VEC_G = VEC4_Y,
    COLOR_VEC_B = VEC4_Z,
    COLOR_VEC_A = VEC4_W,
    COLOR_VEC_LENGTH = VEC4_LENGTH,
    COLOR_VEC_SIZE = VEC4_SIZE
};

// @}

// matrices
// @{

typedef vec2 mat2[2];
typedef vec3 mat3[3];
typedef vec4 mat4[4];

// @}

// VBOs/VAOs and related data
// @{

typedef GLuint
VBO; // vertex buffer object - GPU side object containing a single attribute, of multiple vertices
typedef GLuint
VAO; // vertex array object - GPU side object containing multiple VBOs and thus several vertices with all their attributes
enum VertexAttributeIndex
{
    VERTEX_ATTRIBUTE_POSITION = 0,
    VERTEX_ATTRIBUTE_COLOR = 1,
    VERTEX_ATTRIBUTE_COUNT = 2
};

typedef GLuint EBO;

// @}

// Model upload/assignment for drawing/destruction
// @{

typedef struct Model
{
    VAO vao;
    EBO ebo;
    u32* indexes;
    u32 index_count;
    VBO vbos[VERTEX_ATTRIBUTE_COUNT];
    u32 vert_count;
    vec3* pos; // heap-allocated, accessed only if model needs to be uploaded to the GPU (which is, ideally, rare)
    color_vec* colors; // one color per vert
} Model;

// create and upload to GPU
void model( Model* m, u32 vert_count, u32 index_count, vec3* verts,
            u32* indexes, color_vec color // one color per model
          );
void modelDestroy( Model* m );

void useModel( Model* m );
void drawModel( Model* m );

// @}
