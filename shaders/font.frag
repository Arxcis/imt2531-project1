#version 410

in vec2 Texcoord;
out vec4 outColor;

uniform sampler2D diffuse;

void main()
{
    outColor = texture(diffuse, Texcoord);
    // @temporary @hack found here https://gamedev.stackexchange.com/a/149613 - 04.02.18
    if (outColor.r <= 0.2) {
        discard;
    }

    outColor = vec4(0,0,1,1);

}
