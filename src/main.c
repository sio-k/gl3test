/*
 * Copyright (C) 2017 Sio Kreuzer. All Rights Reserved.
 */

#include "libs.h"

#include "renderer_data.h"
#include "shaders.h"

extern SDL_Window* g_window = NULL;
extern SDL_GLContext g_glcontext = NULL;

void mainLoop();

int main( int argc, char** argv, char** envv )
{
    // library setup
    SDL_Init( SDL_INIT_EVERYTHING );
    atexit( SDL_Quit );

    IMG_Init( IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_JPG );
    atexit( IMG_Quit );

    TTF_Init();
    atexit( TTF_Quit );

    // turn on GL3.0+ support in GLEW
    glewExperimental = GL_TRUE;

    // window/gl setup
    g_window = SDL_CreateWindow(
                   "rush ramp",
                   SDL_WINDOWPOS_CENTERED,
                   SDL_WINDOWPOS_CENTERED,
                   1024,
                   768,
                   SDL_WINDOW_SHOWN
                   | SDL_WINDOW_OPENGL
               );
    assert( g_window != NULL );

    // using GL4.3 core (supported on NV Fermi/400, AMD/ATi HD5000, Intel Haswell and newer)
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_MAJOR_VERSION,
        4
    );
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_MINOR_VERSION,
        3
    );

    SDL_GL_SetAttribute(
        SDL_GL_DOUBLEBUFFER,
        1
    );

    g_glcontext = SDL_GL_CreateContext( g_window );

    // enable vertical sync
    SDL_GL_SetSwapInterval( 1 );

    // we have a GL context so we can finally init GLEW
    glewInit();

    // we want to write fragment values to the back buffer (left because no stereo)
    GLenum draw_buffers[1] = { GL_BACK_LEFT };
    glDrawBuffers( 1, draw_buffers );

    mainLoop();

    // manual destruction
    SDL_GL_DeleteContext( g_glcontext );
    SDL_DestroyWindow( g_window );

    exit( 0 );
}

const vec3 square_pos[4] = {
    {  0.5f,  0.5f, 0.5f },
    { -0.5f,  0.5f, 0.5f },
    {  0.5f, -0.5f, 0.5f },
    { -0.5f, -0.5f, 0.5f }
};
const color_vec square_color = { 0.3f, 0.6f, 0.9f, 1.0f };
const u32 square_indexes[6] = { 0, 1, 2, 1, 2, 3 };
Model square;

i8 mainLoopStep();
void mainLoop()
{
    model( &square, 4, 6, square_pos, square_indexes, square_color );

    // shaders
    const char* vsh_path = "/home/sioclya/c/gl3test/src/shaders/simple.vert";
    const char* fsh_path = "/home/sioclya/c/gl3test/src/shaders/simple.frag";
    ShaderProgram sh;
    shaderProgram( &sh, vsh_path, NULL, NULL, NULL, fsh_path, NULL );

    while ( mainLoopStep() );

    shaderProgramDestroy( &sh );

    modelDestroy( &square );
}

i8 handleInput( u8* key_state );
i8 mainLoopStep()
{
    // process inputs
    // step 1: keyboard state
    i32 num_keys = 0;
    u8* key_state = SDL_GetKeyboardState( &num_keys );
    assert( key_state != NULL );
    // we want arrow keys to be supported, since UP is 82 and last key we need, we check for that
    assert( num_keys >= SDL_SCANCODE_UP );
    if ( handleInput( key_state ) ) {
        // OK, continue
    } else {
        // quit request
        return 0;
    }

    // step 2: quit events
    SDL_Event ev;
    while ( SDL_PollEvent( &ev ) ) {
        switch ( ev.type ) {
            case SDL_QUIT:
                return 0;
                break;
            default:
                break;
        }
    }

    // clear
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // render stuff
    drawModel( &square );

    // present
    SDL_GL_SwapWindow( g_window );
    return 1;
}

i8 handleInput( u8* key_state )
{
    i8 return_value = 1;

    if ( key_state[SDL_SCANCODE_ESCAPE] ) {
        return_value = 0;
    }
    // TODO: handle other keys here

    return return_value;
}
