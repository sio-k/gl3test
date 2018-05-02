#version 150
/*
 * Copyright (C) 2017 Sio Kreuzer. All Rights Reserved.
 */

// NOTE: shader typed up while drunk. Might contain some errors.

attribute vec3 in_Position;
attribute vec4 in_Color;

out vec4 ex_Color;

void main( void )
{
    gl_Position = vec4( in_Position.x + 0.25, in_Position.y - 0.25, in_Position.z, 1.0 );
    ex_Color = in_Color;
}
