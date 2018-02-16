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
    gl_Position = scale * move * vec4(position, 0,1);
    gl_PointSize = pointSize;

    vec3 c = vec3(
        sin(time*10*(position.x-16)*0.25+   (position.y-16)*0.125)+1,
        cos(time*10*(position.x-16)*0.125+  (position.y-16)*0.0625)+1,
        sin(time*10*(position.x-16)*0.03125+(position.y-16)*0.25)+1)*0.375;

    Color = vec4(c, color.a);

    Texcoord = texcoord;
}
