#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vuv;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in ivec4 vBoneIDs;
layout (location = 4) in vec4 vWeights;

out vec2 uv;
out vec3 normal;
out vec3 fragPos;

uniform mat4 MVP;
uniform mat4 M;
const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];

void main(){
    mat4 BoneTransform = gBones[vBoneIDs[0]] * vWeights[0];
    BoneTransform += gBones[vBoneIDs[1]] * vWeights[1];
    BoneTransform += gBones[vBoneIDs[2]] * vWeights[2];
    BoneTransform += gBones[vBoneIDs[3]] * vWeights[3];
    vec4 position = BoneTransform * vec4(vPosition, 1);
    gl_Position = MVP * position;
    uv = vuv;
    normal = vec3(M*BoneTransform*vec4(vNormal, 0));
    fragPos = vec3(M*position);
}