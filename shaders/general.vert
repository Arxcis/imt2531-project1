#version 410

in vec2 position;
in vec4 color;
in vec2 texcoord;

out vec4 Color;
out vec2 Texcoord;

uniform mat4 scale = mat4(
1.0/14.0,   0,          0,  0,
0,          1.0/14.0,   0,  0,
0,          0,          1,  0,
0,          0,          0,  1);

uniform mat4 move = mat4(
1,          0,          0,  0,
0,          1,          0,  0,
0,          0,          1,  0,
-13.5,          -15,    0,  1);

uniform float pointSize = 1.0;

void main()
{
    gl_Position = scale * move * vec4(position, 0.0, 1.0);
    gl_PointSize = pointSize;

    Color = color;
    Texcoord = texcoord;
}
