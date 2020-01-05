#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
#define LIGHT_COUNT 4

uniform vec3 viewPos;
uniform Material material;
uniform Light light[LIGHT_COUNT];

void main()
{
    // ambient
    vec3 ambient = light[0].ambient * texture(material.diffuse, TexCoords).rgb;
    // diffuse 
    vec3 diffuse = vec3(0.0f);
    vec3 norm = normalize(Normal);
    for(int i = 0; i < LIGHT_COUNT; i++){
        vec3 lightDir = normalize(light[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        diffuse += light[0].diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    }

    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
} 