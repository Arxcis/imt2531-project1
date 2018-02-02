#version 410

in vec2 position;
//in vec4 color;
out vec4 color_from_vshader;

//uniform
uniform vec4 color
void main() {
    gl_Position = position;
    color_from_vshader = color;
}
