#version 150 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float quadSize = 0.075; //set this uniform based on screenspace->worldspace

/*
    0 is origin from vertexshader

    2-------3
    |  \    |
    |    \  |
    0-------1
*/

void main()
{
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(quadSize, 0.0, 0.0, 0.0);
    EmitVertex();


    gl_Position = gl_in[0].gl_Position + vec4(0.0, quadSize, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(quadSize, quadSize, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
