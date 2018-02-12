#version 410

in vec2 position;

uniform mat4 worldScale = mat4(
2,0,0,0,
0,2,0,0,
0,0,1,0,
0,0,0,1);

uniform mat4 view2 = mat4(
1,0,0,2,
0,1,0,0,
0,0,1,0,
0,0,0,1);

void main() {
    gl_Position =  worldScale * vec4(position,0,1);
}
