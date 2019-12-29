#version 330 core

out vec4 color;

in DATA {
	vec3 pos;
	vec3 normal;
}fs_in;

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
	//ambient
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;

	//diffuse
	vec3 norm = normalize(fs_in.normal);
	vec3 lightDir = normalize(lightPos - fs_in.pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	color = vec4((ambient + diffuse) * objectColor, 1.0);
}