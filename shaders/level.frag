#version 410

out vec4 out_color;

uniform vec4 floor_color = vec4(0.0, 0.0, 0.0, 1.0);

void main()
{
    out_color = floor_color;
}
