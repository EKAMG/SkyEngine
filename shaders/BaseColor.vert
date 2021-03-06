#version 450 core

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec4 inNormal;
layout (location = 2) in vec4 inColor;

layout (location = 0) out vec3 outPos;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec4 outColor;

layout (set = 0, binding = 0) uniform ObjectInfo {
    mat4 worldMatrix;
    mat4 inverseTranspose;
} objectInfo;

layout (set = 0, binding = 1) uniform ViewInfo {
    mat4 viewMatrix;
    mat4 viewProject;
} viewInfo;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main(void)
{
    vec4 worldPos = objectInfo.worldMatrix * inPos;
    gl_Position = viewInfo.viewProject * worldPos;

    outNormal = normalize(mat3(objectInfo.inverseTranspose) * inNormal.xyz);
    outColor = inColor;
    outPos = worldPos.xyz / worldPos.w;
}