/*
 * Copyright (C) 2017 Sio Kreuzer. All Rights Reserved.
 */

#include "shaders.h"

char* readFileIntoString( const char* path )
{
    if ( path == NULL ) {
        return NULL;
    }

    char* str = NULL;
    FILE* f = fopen( path, "r" );
    if ( !f ) {
        return str;
    }

    fseek( f, 0, SEEK_END );
    i32 size = ftell( f ) + 1024;
    rewind( f );
    str = calloc( size, sizeof( char ) );

    i32 i = 0;
    char c = 'q'; // non-EOF starting value
    while ( c != EOF ) {
        c = getc( f );
        str[i] = c;
        i++;
    }

    fclose( f );
    return str;
}

void compileShader( GLuint sh )
{
    glCompileShader( sh );
    i32 was_compiled = 0;
    glGetShaderiv( sh, GL_COMPILE_STATUS, &was_compiled );
    if ( was_compiled == 0 ) {
        printf( "ERROR DURING FRAGMENT SHADER COMPILATION:\n" );
        i32 len = 0;
        glGetShaderiv( sh, GL_INFO_LOG_LENGTH, &len );
        char* shader_info_log = calloc( len, sizeof( char ) );
        glGetShaderInfoLog( sh, len, &len, shader_info_log );
        printf( "%s\n", shader_info_log );
        free( shader_info_log );
        abort();
    }
}

void linkShaderProgram( GLuint shader_program )
{
    glLinkProgram( shader_program );
    i32 is_linked = 0;
    glGetProgramiv( shader_program, GL_LINK_STATUS, &is_linked );
    if ( is_linked == 0 ) {
        printf( "ERROR DURING SHADER LINKING:\n" );
        i32 len = 0;
        glGetShaderiv( shader_program, GL_INFO_LOG_LENGTH, &len );
        char* shader_info_log = calloc( len, sizeof( char ) );
        glGetShaderInfoLog( shader_program, len, &len, shader_info_log );
        printf( "%s\n", shader_info_log );
        free( shader_info_log );
        abort();
    }
}

void shader(
    Shader* sh,
    GLenum shader_type,
    const char* path_to_source_file,
    GLuint shader_program
)
{
    assert( sh != NULL );
    if ( path_to_source_file == NULL ) {
        // no source file, no shader id, we write this value to make following commands safe
        sh -> num = 0;
        return;
    }
    sh -> num = glCreateShader( shader_type );
    char* sh_src = readFileIntoString( path_to_source_file );
    if ( sh_src == NULL ) {
        sh -> num = 0;
        return;
    }
    i32 sh_src_size = strlen( sh_src );
    glShaderSource( sh -> num, 1, &sh_src, &sh_src_size );
    compileShader( sh -> num );
    glAttachShader( shader_program, sh -> num );
    free( sh_src );
}

void shaderDetach( Shader* sh, GLuint shader_program )
{
    if ( sh -> num == 0 ) {
        // do nothing, no shader
    } else {
        if ( shader_program ) {
            glDetachShader( shader_program, sh -> num );
        }
    }
}

void shaderDelete( Shader* sh )
{
    if ( sh -> num == 0 ) {
        // do nothing, no shader
    } else {
        glDeleteShader( sh -> num );
    }
}

void shaderProgram(
    ShaderProgram* sh,
    const char* vert_src,
    const char* tesc_src,
    const char* tese_src,
    const char* geom_src,
    const char* frag_src,
    const char* compute_src
)
{
    assert( sh != NULL );
    sh -> prog = glCreateProgram();

    // bind attribute locations
    // NOTE: has to be updated as new attributes we're feeding to the vertex shader are introduced
    glBindAttribLocation( sh -> prog, VERTEX_ATTRIBUTE_POSITION, "in_Position" );
    glBindAttribLocation( sh -> prog, VERTEX_ATTRIBUTE_COLOR, "in_Color" );

    shader( &( sh -> vert ), GL_VERTEX_SHADER, vert_src, sh -> prog );
    shader( &( sh -> tesc ), GL_TESS_CONTROL_SHADER, tesc_src, sh -> prog );
    shader( &( sh -> tese ), GL_TESS_EVALUATION_SHADER, tese_src, sh -> prog );
    shader( &( sh -> geom ), GL_GEOMETRY_SHADER, geom_src, sh -> prog );
    shader( &( sh -> frag ), GL_FRAGMENT_SHADER, frag_src, sh -> prog );
    //shader( &( sh -> compute ), GL_COMPUTE_SHADER, comp_src, sh -> prog );

    linkShaderProgram( sh -> prog );
    glUseProgram( sh -> prog );

    shaderDelete( &( sh -> vert ) );
    shaderDelete( &( sh -> tesc ) );
    shaderDelete( &( sh -> tese ) );
    shaderDelete( &( sh -> geom ) );
    shaderDelete( &( sh -> frag ) );
    //shaderDelete( &( sh -> compute ) );
}

void shaderProgramDestroy( ShaderProgram* sh )
{
    assert( sh != NULL );
    glUseProgram( 0 );
    shaderDetach( &( sh -> vert ), sh -> prog );
    shaderDetach( &( sh -> tesc ), sh -> prog );
    shaderDetach( &( sh -> tese ), sh -> prog );
    shaderDetach( &( sh -> geom ), sh -> prog );
    shaderDetach( &( sh -> frag ), sh -> prog );
    //shaderDetach( &( sh -> compute ), sh -> prog );
    glDeleteProgram( sh -> prog );
}

void useShaderProgram( ShaderProgram* sh )
{
    if ( sh != NULL ) {
        glUseProgram( sh -> prog );
    } else {
        glUseProgram( 0 );
    }
}
