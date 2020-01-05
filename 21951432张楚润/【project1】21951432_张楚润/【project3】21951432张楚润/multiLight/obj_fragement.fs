// 箱子 片断着色器
#version 330 core

// 材质结构体
struct Material
{
    sampler2D diffuse;   //  漫反射光照下物体的颜色
    sampler2D specular;  // 物体受到的镜面光照的影响的颜色
    float shininess;    // 高光的散射/半径
};


struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct Light {
    vec3 position;      // 光源位置
    //vec3 direction;

    vec3 ambient;       // 环境光
    vec3 diffuse;           // 漫反射光
    vec3 specular;      // 镜面光
                                // 计算衰减值
    float constant;     // 常数项Kc
    float linear;           // 一次项Kl
    float quadratic;        // 二次项KqKq

    //float cutOff;     //  内圆切光角
    //float outerCutOff;        // add 外圆切光角
};

in vec3 IN_ObjPos;      // 顶点位置
in vec3 IN_Normal;      // 法向量
in vec2 IN_TexCoords;   // 纹理

out vec4 color;

#define NR_POINT_LIGHTS 4
uniform vec3 viewPos;
uniform DirLight dirLight;      //
uniform Material material;
uniform Light lights[NR_POINT_LIGHTS];      // 光源数组


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 normal = normalize(IN_Normal)  ;
    vec3 viewDir = normalize(viewPos - IN_ObjPos);

    vec3 result = CalcDirLight(dirLight, normal, viewDir);

    for(int i = 0; i< NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(lights[i], normal, IN_ObjPos, viewDir);
    }

    color = vec4(result, 1.0);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);        // 光线方向

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, IN_TexCoords));// 环境光
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, IN_TexCoords));// 漫反射光
    vec3 specular = light.specular * spec * vec3(texture(material.specular, IN_TexCoords));// 镜面光

    return vec3(ambient + diffuse + specular);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - IN_ObjPos); // 光源和片段位置之间的方向向量
    float diff = max(dot(normal, lightDir), 0.0);   // 点乘， 来计算光对当前片段的实际的散射影响  大于90度， 点乘的结果就会变成负数， max 函数返回两个参数之间较大的参数， 从而保证散射因子不会变成负数。
    vec3 reflectDir = reflect(-lightDir, normal);  // 计算反射向量和视线方向的角度， 如果之间的角度越小， 那么镜面光的作用就会越大
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, IN_TexCoords));// 环境光
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, IN_TexCoords));// 漫反射光
    vec3 specular = light.specular * spec * vec3(texture(material.specular, IN_TexCoords));// 镜面光

    float distance = length(light.position - IN_ObjPos);    // 光源到物体的距离
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    // 衰减因子

    ambient *= attenuation;     // 所有的光线都与衰减因子相乘
    diffuse *= attenuation;
    specular *= attenuation;

    return vec3(ambient + diffuse + specular);  // 最后的输出颜色。
}
