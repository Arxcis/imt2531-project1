#version 410

in vec4 Color;
in vec2 Texcoord;

out vec4 out_color;

uniform vec4 cheese_color = vec4(1.0, 0.0, 0.0, 1.0);

void main()
{
    out_color = cheese_color * Color.a;
}
