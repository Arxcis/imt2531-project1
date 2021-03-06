#version 410

in vec4 Color;
in vec2 Texcoord;

out vec4 out_color;

uniform vec4 floor_color = vec4(0.0, 0.0, 0.0, 1.0);

void main()
{
    out_color = floor_color;
}
