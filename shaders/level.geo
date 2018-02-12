#version 150 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float quadSize = 0;
/*
    0 is origin from vertexshader

    2-------3
    |  \    |
    |    \  |
    0-------1


    If needed, look into using this structure:

    2-------3
    | \     |
    |   O   |
    |     \ |
    0-------1

*/
void main()
{
    float q = quadSize*0.5;

    gl_Position = gl_in[0].gl_Position - vec4(q,q,0,0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(q, -q, 0.0, 0.0);
    EmitVertex();


    gl_Position = gl_in[0].gl_Position + vec4(-q, q, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(q, q, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
