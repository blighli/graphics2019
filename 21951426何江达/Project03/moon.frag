#version 330 core
in vec3 normal;
in vec3 position;

out vec4 fragColor;
uniform vec3 viewPos;
in vec2 TexCoords;
uniform sampler2D texture_diffuse1;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLight;

struct Material
{
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shininess;
};
uniform Material material;
uniform int picked;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(void)
{
    // properties
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - position);

    vec3 result= CalcPointLight(pointLight, norm, position, viewDir);

    if(picked==1)
        fragColor =vec4(result,0.0);
    else fragColor=vec4(result,1.0);

}
// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading

    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture2D(texture_diffuse1, TexCoords));

    vec3 diffuse = light.diffuse * diff * vec3(texture2D(texture_diffuse1, TexCoords));

    vec3 specular = light.specular * spec * vec3(texture2D(texture_diffuse1, TexCoords));

    ambient *= attenuation;
//    diffuse *= attenuation;
    specular *= attenuation;

    return (/*ambient +*/ diffuse /*+ specular*/);
}
