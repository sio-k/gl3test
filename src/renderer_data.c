/*
 * Copyright (C) 2017 Sio Kreuzer. All Rights Reserved.
 */

#include "renderer_data.h"

u32 findVert( vec3 tofind, vec3* to_find_in, u32 amount )
{
    for ( u32 i = 0; i < amount; i++ ) {
        if ( ( tofind[0] == to_find_in[i][0] )
                && ( tofind[1] == to_find_in[i][1] )
                && ( tofind[2] == to_find_in[i][2] )
           ) {
            return i;
        }
    }
    return ( u32 )( -1 );
}

void model( Model* m, u32 vert_count, u32 index_count, vec3* verts,
            u32* indexes,
            color_vec color )
{
    assert( m != NULL );
    memset( m, 0, sizeof( Model ) );

    m -> index_count = index_count;
    m -> vert_count = vert_count;

    m -> pos = malloc( vert_count * VEC3_SIZE );
    memcpy( m -> pos, verts, vert_count * VEC3_SIZE );
    m -> colors = malloc( vert_count * 3 * COLOR_VEC_SIZE );
    for ( u32 i = 0; i < vert_count; i++ ) {
        memcpy( m -> colors, color, COLOR_VEC_SIZE );
    }
    m -> indexes = malloc( index_count * sizeof( u32 ) );
    memcpy( m -> indexes, indexes, index_count );
    #if 0
    // OK, we have tris, now copy their verts into the vert array one by one,
    // recording duplicates along the way and building the EBO index buffer as we go

    u32 amount_of_verts = 0;
    for ( u32 i = 0; i < tri_count; i++ ) {
        for ( i32 j = 0; j < 3; j++ ) {
            // check if vert already in position buffer, make record in index buffer
            u32 found_vert = findVert( tris[i][j], m -> pos, amount_of_verts );
            if ( found_vert != ( u32 )( -1 ) ) {
                m -> indexes[i][j] = found_vert;
            } else {
                // vert not found in buffer, insert
                memcpy( m -> pos[amount_of_verts], tris[i][j], VEC3_SIZE );
                memcpy( m -> colors[amount_of_verts], color, sizeof( color_vec ) );
                m -> indexes[i][j] = amount_of_verts;
                amount_of_verts++;
            }
        }
    }
    m -> vert_count = amount_of_verts;
    #endif
    glGenBuffers( VERTEX_ATTRIBUTE_COUNT, m -> vbos );
    glBindBuffer( GL_ARRAY_BUFFER, m -> vbos[VERTEX_ATTRIBUTE_POSITION] );
    glBufferData(
        GL_ARRAY_BUFFER,
        m -> vert_count * VEC3_SIZE,
        m -> pos,
        GL_STATIC_DRAW
    );
    glBindBuffer( GL_ARRAY_BUFFER, m -> vbos[VERTEX_ATTRIBUTE_COLOR] );
    glBufferData(
        GL_ARRAY_BUFFER,
        m -> vert_count * COLOR_VEC_SIZE,
        m -> colors,
        GL_STATIC_DRAW
    );

    glGenVertexArrays( 1, &( m -> vao ) );
    glBindVertexArray( m -> vao );
    glBindBuffer( GL_ARRAY_BUFFER, m -> vbos[VERTEX_ATTRIBUTE_POSITION] );
    glVertexAttribPointer(
        VERTEX_ATTRIBUTE_POSITION,
        VEC3_LENGTH,
        GL_FLOAT,
        GL_FALSE,
        VEC3_SIZE,
        NULL
    );
    glEnableVertexAttribArray( VERTEX_ATTRIBUTE_POSITION );

    glBindBuffer( GL_ARRAY_BUFFER, m -> vbos[VERTEX_ATTRIBUTE_COLOR] );
    glVertexAttribPointer(
        VERTEX_ATTRIBUTE_COLOR,
        COLOR_VEC_LENGTH,
        GL_FLOAT,
        GL_FALSE,
        COLOR_VEC_SIZE,
        NULL
    );
    glEnableVertexAttribArray( VERTEX_ATTRIBUTE_COLOR );

    glGenBuffers( 1, &( m -> ebo ) );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m -> ebo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, m -> index_count, m -> indexes,
                  GL_STATIC_DRAW );

    // bind element buffer object to VAO
    glBindVertexArray( m -> vao );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m -> ebo );
}

void modelDestroy( Model* m )
{
    assert( m != NULL );

    glBindVertexArray( m -> vao );
    for ( i32 i = 0; i < VERTEX_ATTRIBUTE_COUNT; i++ ) {
        glDisableVertexAttribArray( i );
    }

    glDeleteBuffers( VERTEX_ATTRIBUTE_COUNT, m -> vbos );
    glDeleteBuffers( 1, &( m -> ebo ) );
    glDeleteVertexArrays( 1, &( m -> vao ) );

    free( m -> indexes );
    free( m -> pos );
    free( m -> colors );
}

void useModel( Model* m )
{
    assert( m != NULL );
    glBindVertexArray( m -> vao );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m -> ebo );
}

void drawModel( Model* m )
{
    assert( m != NULL );
    useModel( m );
    glDrawElements( GL_TRIANGLES, m -> index_count, GL_UNSIGNED_INT, 0 );
}
