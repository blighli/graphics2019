#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture;

void main(void)
{
    FragColor = texture2D(texture,TexCoords);
}
