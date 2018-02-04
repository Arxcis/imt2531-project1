#version 410

in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texPacman;
void main()
{
    outColor = texture(texPacman, Texcoord);

    // @temporary @hack found here https://gamedev.stackexchange.com/a/149613 - 04.02.18
    if (outColor.a <= 0.0) {
        discard;
    }
}