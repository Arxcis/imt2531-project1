#version 410

in vec2 position;
in vec4 color;
in vec2 texcoord;

out vec4 Color;
out vec2 Texcoord;

uniform mat4 scale;
uniform mat4 move;

uniform float pointSize = 1.0;
uniform float time;

void main()
{
    gl_Position = scale * move * vec4(position, 0.0, 1.0);
    gl_PointSize = pointSize;

    Color = color;
    Texcoord = texcoord;
}
