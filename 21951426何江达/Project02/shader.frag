#version 330 core
in vec3 normal;
in vec3 position;

out vec4 fragColor;
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct Light
{
    vec4 position;
    vec4 intensity;
};
uniform Light light;

struct Material
{
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shininess;
};
uniform Material material;
uniform vec3 viewPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
void main(void)
{
    // normal
    vec3 n=normalize(normal);
    // lightDir
    vec3 s=normalize(light.position.xyz-position);
    // viewDir
    vec3 v=normalize(viewPos-position);
//    vec3 h=normalize(v+s);
    // reflectDir
    vec3 r=reflect(-s,n);

    // ambient
    vec3 ambient=material.Ka;
    // diffuse
    vec3 diffuse=material.Kd*max(dot(n,s),0.0);
    // specular
    vec3 specular=material.Ks * pow(max(dot(v,r),0),material.shininess);

//    ambient*=vec3(texture2D(texture_normal1,TexCoords));
    diffuse*=vec3(texture2D(texture_diffuse1,TexCoords));
    specular*=vec3(texture2D(texture_specular1,TexCoords));

    fragColor =
texture2D(texture_diffuse1,TexCoords);

}
