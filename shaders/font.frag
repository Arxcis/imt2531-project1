#version 410

in vec2 Texcoord;
out vec4 outColor;

uniform sampler2D diffuse;

void main()
{
    outColor = texture(diffuse, Texcoord);
    outColor.r = 0;
    outColor.g = 0;
   outColor.b = 1;
    //outColor = vec4(0,0,1,1);
}
