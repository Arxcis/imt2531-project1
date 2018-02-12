#version 410
in vec2 position;
in vec2 texcoord;
out vec2 Texcoord;

uniform mat4 view = mat4(
1.0/14.0,   0,          0,  0,
0,          1.0/14.0,   0,  0,
0,          0,          1,  0,
0,          0,          0,  1);

void main()
{
    Texcoord = texcoord;
    gl_Position = view * vec4(position, 0.0, 1.0);
}
