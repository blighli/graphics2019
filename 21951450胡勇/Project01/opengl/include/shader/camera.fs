#version 330 core
out vec4 FragColor;

in vec3 ourColor;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = vec4(ourColor, 1.0f);
}