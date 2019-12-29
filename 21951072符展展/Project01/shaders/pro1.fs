#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D container;

void main()
{
	vec3 color = texture(container,TexCoord).rgb;
	FragColor = vec4(color, 1.0);
	//FragColor = vec4(1.0);
}