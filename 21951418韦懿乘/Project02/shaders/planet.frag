#version 330
out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform sampler2D albedo_map;
uniform sampler2D specular_map;
uniform sampler2D cloud_map;
uniform vec3      lightPos;
uniform vec3      viewPos;

void main() {
    vec3 albedo = texture(albedo_map, TexCoords).rgb;
    vec3 cloud = texture(cloud_map, TexCoords).rgb;
    float specular_map_val = texture(specular_map, TexCoords).r;
    // ambient
    vec3 base = albedo + cloud;
    vec3 ambient = 0.1 * base;
    // diffuse
    vec3 lightDir = normalize(lightPos - WorldPos);
    vec3 normal   = normalize(Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * base;
    // specular
    vec3 viewDir = normalize(viewPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.3) * spec * specular_map_val;

    vec3 color = ambient + diffuse + specular;
    // gamma:
    color = pow(color, vec3(1.0 / 2.2));
    FragColor = vec4(color, 1.0);
}
