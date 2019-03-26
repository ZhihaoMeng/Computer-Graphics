#version 330 core

out vec4 frag_color;

in vec2 TexCoord;

//uniform vec4 color;

uniform sampler2D ourTexture;

void main()
{
    //frag_color = color;
    frag_color = texture(ourTexture, TexCoord);
}
