#version 410

in vec2 position;



uniform mat4 view = mat4(
1.0/14.0,   0,          0,  0,
0,          1.0/14.0,   0,  0,
0,          0,          1,  0,
0,          0,          0,  1);

void main() {

    gl_Position =  view * vec4(position,0,1) ;
}
