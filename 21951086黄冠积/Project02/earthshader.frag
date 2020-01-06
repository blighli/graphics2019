#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;

struct LightPoint{
	vec3 LightPos;
	vec3 LightColor;
	float constant;
	float linear;
	float quadratic;
};


uniform LightPoint LightP;
uniform sampler2D texture1;
uniform vec3 earth_core_pos;
uniform vec3 ambident;
uniform vec3 cameraPos;

void main()
{
	// ambident
	vec3 ambidentColor = ambident * texture(texture1, TexCoord).rgb;

	// diffuse
	float dist = length(LightP.LightPos - FragPos);
	float attenuation = 1.0 / (LightP.constant + LightP.linear * dist +  LightP.quadratic * (dist * dist));
	float diffIntensity = max(dot(normalize(LightP.LightPos-FragPos),normalize(FragPos-earth_core_pos)),0) * attenuation;
	vec3 diffuseColor = diffIntensity * LightP.LightColor * 10 * texture(texture1, TexCoord).rgb;

	// specular
	vec3 dirToCamera = normalize(cameraPos-FragPos);
	vec3 R = normalize(reflect(normalize(FragPos-LightP.LightPos),normalize(FragPos-earth_core_pos)));
	float specIntensity = pow(max(dot(R,dirToCamera),0),32) * attenuation;
	vec3 specularColor = specIntensity * LightP.LightColor * 10 * texture(texture1,TexCoord).rgb ;

	FragColor = vec4(ambidentColor + diffuseColor + specularColor,1.0f);
}