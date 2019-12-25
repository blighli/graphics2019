#version 330 core
out vec4 fragColor;

in vec2 uv;
in vec3 normal;
uniform sampler2D diffuseTexture;
uniform vec3 lightDir;

void main(){
    //light
    vec3 lightColor = vec3(1,1,1);
    float lightPower = 1.0;
    //material
    float d = texture(diffuseTexture, uv).r;
    vec3 materialDiffuseColor = vec3(d,d,d);
    //compute
    vec3 n = normalize(normal);
    vec3 l = normalize(lightDir);
    float cosTheta = clamp(dot(n,l),0,1);
    //color
    vec3 diffuseColor = materialDiffuseColor*lightColor*lightPower*cosTheta;
    fragColor = vec4(diffuseColor, 1.0);
    //fragColor = vec4(n, 1.0);
}