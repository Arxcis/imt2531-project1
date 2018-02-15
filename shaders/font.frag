#version 410

in vec4 Color;
in vec2 Texcoord;
out vec4 outColor;

uniform sampler2D diffuse;

void main()
{
    outColor = texture(diffuse, Texcoord);

    // Toggling text on off if using this
    outColor.a *= Color.a; 

    outColor.rgb = Color.rgb;
}
