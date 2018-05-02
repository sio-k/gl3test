#version 430 core
/*
 * Copyright (C) 2017 Sio Kreuzer. All Rights Reserved.
 */

// dead simple fragment shader

// needed for some drivers apparently
//precision highp float;

in vec4 ex_color;
layout( location = 0 ) out vec4 out_frag_color;

void main( void )
{
    out_frag_color = vec4( ex_color );
}
