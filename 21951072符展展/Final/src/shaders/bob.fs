#version 330 core
out vec4 fragColor;
in vec2 uv;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D diffuseTexture;
//uniform sampler2D specularTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;


void main(){
    //result
    vec3 result = vec3(0);
    //light
    vec3 lightColor = vec3(1.0);
    float lightPower = 4.0f;

    //material
    vec3 diffuseMaterial = texture(diffuseTexture, uv).rgb;
    //vec3 specularMaterial = texture(specularTexture, uv).rgb;
    //compute
    vec3 n = normalize(normal);
    vec3 l = normalize(lightPos-fragPos);
    vec3 v = normalize(viewPos-fragPos);
    vec3 h = normalize(l+v);
    float cosTheta = clamp(dot(n,l),0,1);
    float cosAlpha = clamp(dot(n,h),0,1);
    float d = length(lightPos-fragPos);
    float attenuation = 1.0/(d);
    //color
    vec3 ambientColor = diffuseMaterial*vec3(0.1)*lightColor;
    vec3 diffuseColor = diffuseMaterial*lightColor*cosTheta;
    //vec3 specularColor = specularMaterial*lightColor*pow(cosAlpha,16)*vec3(0.5);


    result += ambientColor+diffuseColor*attenuation*lightPower;
    fragColor = vec4(result, 1.0);
    //fragColor = vec4(1.0);
}