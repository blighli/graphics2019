#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture samplers
uniform sampler2D ourTexture;

void main()
{
	//color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
	color = texture(ourTexture, TexCoord);
}