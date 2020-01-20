#version 330 core
out vec4 fragColor;
in vec2 uv;
in vec3 normal;
in vec3 fragPos;
uniform sampler2D diffuseTexture;

void main(){
    vec3 color = texture(diffuseTexture, uv).rgb;
    fragColor = vec4(color, 1.0);
    //fragColor = vec4(1.0);
}