#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform vec3 cameraPos;
uniform vec3 ambient;

struct LightPoint{
	vec3 Pos;
	vec3 color;
	vec3 dirToLight;
	float constant;
	float linear;
	float quadratic;
};

struct LightSpot{
	vec3 Pos;
	vec3 color;
	vec3 dirToLight;
	float constant;
	float linear;
	float quadratic;
	float cosPhy;
	float cosInnerPhy;
	float cosOutterPhy;
};

struct lightDirectional{
	vec3 Pos;
	vec3 color;
	vec3 dirToLight;
};

uniform LightSpot lightS;
uniform lightDirectional lightD;
uniform LightPoint lightP;


vec3 CalcLightDirectional(lightDirectional light, vec3 uNormal, vec3 dirToCamera){
	
	vec3 result;

	// ambident
	vec3 ambidentColor = ambient * texture(texture_diffuse1, TexCoords).rgb;

	// diffuse
	float diffIntensity = max( dot( light.dirToLight, uNormal) , 0);
	vec3 diffColor = diffIntensity * light.color * texture(texture_diffuse1,TexCoords).rgb;

	// specular
	vec3 R = normalize(reflect(-light.dirToLight,uNormal));
	float specIntensity = pow(max(dot(R,dirToCamera),0),32);
	vec3 specColor = specIntensity * light.color * texture(texture_diffuse1,TexCoords).rgb;

	result += ambidentColor + diffColor + specColor;
	return result;
}

vec3 CalcLightPoint(LightPoint light, vec3 uNormal, vec3 dirToCamera){
	vec3 result;
	float dist    = length(light.Pos - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist +  light.quadratic * (dist * dist));
//	attenuation = 1.0f;

	float diffIntensity = max( dot( normalize(light.Pos - FragPos), uNormal) , 0) * attenuation;
	vec3 diffColor = diffIntensity * light.color * texture(texture_diffuse1,TexCoords).rgb;

	vec3 R = normalize(reflect(normalize(FragPos-light.Pos),uNormal));
	float specIntensity = pow(max(dot(R,dirToCamera),0),32) * attenuation;
	vec3 specColor = specIntensity * light.color * texture(texture_diffuse1,TexCoords).rgb ;


	result += diffColor + specColor;
//	result += specColor;
//	result += diffColor;
//	result += ambient*texture(texture_diffuse1,TexCoords).rgb;
	return result;
}

vec3 CalcLightSpot(LightSpot light, vec3 uNormal, vec3 dirToCamera){
	vec3 result = vec3(0,0,0);

	float dist    = length(light.Pos - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist +  light.quadratic * (dist * dist));
//	attenuation = 1.0f;

	float cosTheta = dot(normalize(FragPos - light.Pos), light.dirToLight);
	float spotRatio;
	if(cosTheta>lightS.cosInnerPhy){
		spotRatio = 1.0f;
	}
	else if(cosTheta > lightS.cosOutterPhy){
		spotRatio = (cosTheta - lightS.cosOutterPhy) / (lightS.cosInnerPhy - lightS.cosOutterPhy);
//		spotRatio = 0;
	}
	else{
		spotRatio = 0;
	}


	float diffIntensity = max( dot( normalize(light.Pos - FragPos), uNormal) , 0) * attenuation  * spotRatio;
	vec3 diffColor = diffIntensity * light.color * texture(texture_diffuse1,TexCoords).rgb;

	vec3 R = normalize(reflect(light.dirToLight,uNormal));
	float specIntensity = pow(max(dot(R,dirToCamera),0),32) * attenuation  * spotRatio;
	vec3 specColor = specIntensity * light.color * texture(texture_diffuse1,TexCoords).rgb ;



	return result += diffColor + specColor;
}

void main()
{    
	vec3 finalResult = vec3(0,0,0);
	vec3 uNormal = normalize(Normal);
	vec3 dirToCamera = normalize( cameraPos - FragPos );

	finalResult += CalcLightDirectional( lightD, uNormal, dirToCamera );
	finalResult += CalcLightPoint( lightP, uNormal, dirToCamera );
	finalResult += CalcLightSpot( lightS, uNormal, dirToCamera );

//	FragColor = vec4(finalResult,1.0f);

//	FragColor = vec4(CalcLightSpot( lightS, uNormal, dirToCamera ),1.0f);
//	FragColor = vec4(CalcLightDirectional( lightD, uNormal, dirToCamera ),1.0f);
	FragColor = vec4(finalResult,1.0f);

//    FragColor = texture(texture_diffuse1, TexCoords);
}