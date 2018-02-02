#version 150 core

layout(points) in;
layout(quads, max_vertices = 4) out;

uniform half quadSize

void main()
{
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(quadSize, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(quadSize, quadSize, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.0, quadSize, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
