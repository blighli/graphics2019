#version 330 core
in vec3 normal;
in vec3 position;

out vec4 fragColor;
uniform vec3 viewPos;
in vec2 TexCoords;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
struct DirLight
{
        vec3 direction;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
};

struct SpotLight
{
        vec3 position;
        vec3 direction;
        float cutOff;
        float outerCutOff;

        float constant;
        float linear;
        float quadratic;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
};

struct PointLight
{
        vec3 position;

        float constant;
        float linear;
        float quadratic;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
};
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform DirLight dirLight;

struct Material
{
        vec3 Ka;
        vec3 Kd;
        vec3 Ks;
        float shininess;
};
uniform Material material;
uniform int picked;
uniform int sun;
uniform int moon;
uniform int earth;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(void)
{
        // properties
        vec3 norm = normalize(normal);
        vec3 viewDir = normalize(viewPos - position);
        vec3 result=vec3(0,0,0);

        if (moon==0)
                result += CalcPointLight(pointLight, norm, position, viewDir);
        if (sun==0)
                result += CalcDirLight(dirLight, norm, viewDir);
        if(earth==0)
            result += CalcSpotLight(spotLight, norm, position, viewDir);
        fragColor = vec4(result, 1.0);

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
        vec3 specular = light.specular * spec * vec3(texture2D(texture_specular1, TexCoords));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
        return (ambient + diffuse + specular);
}
// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
        vec3 lightDir = normalize(-light.direction);
        // diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);
        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        // combine results
        vec3 ambient = light.ambient * vec3(texture2D(texture_diffuse1, TexCoords));
        vec3 diffuse = light.diffuse * diff * vec3(texture2D(texture_diffuse1, TexCoords));
        vec3 specular = light.specular * spec * vec3(texture2D(texture_specular1, TexCoords));
        return (ambient + diffuse + specular);
}


// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
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
        // spotlight intensity
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        // combine results
        vec3 ambient = light.ambient * vec3(texture2D(texture_diffuse1, TexCoords));
        vec3 diffuse = light.diffuse * diff * vec3(texture2D(texture_diffuse1, TexCoords));
        vec3 specular = light.specular * spec * vec3(texture2D(texture_specular1, TexCoords));
        ambient *= attenuation * intensity;
        diffuse *= attenuation * intensity;
        specular *= attenuation * intensity;
        return (ambient + diffuse + specular);
}
