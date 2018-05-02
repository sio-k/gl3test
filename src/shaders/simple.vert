#version 430 core
/*
 * Copyright (C) 2017 Sio Kreuzer. All Rights Reserved.
 */

// dead simple vertex shader
layout( location = 0 ) in vec3 in_position; // attr index 0
layout( location = 1 ) in vec4 in_color; // attr index 1

out vec4 ex_color; // output for next shader in chain

void main( void )
{
    gl_Position = vec4( in_position.x, in_position.y, in_position.z, 1.0 );
    
    ex_color = in_color;
}
