#version 330 core
in vec3 normal;

out vec4 fragColor;

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
uniform int picked;
void main(void)
{
        if (picked == 1)
                fragColor = texture2D(texture_diffuse1, TexCoords) * 0.5;
        else
                fragColor = texture2D(texture_diffuse1, TexCoords);
//        fragColor=vec4(1,1,1,1);
}
