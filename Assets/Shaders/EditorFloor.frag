#define BANG_FRAGMENT
#include "Common.glsl"

in vec3 B_FIn_Position;
in vec3 B_FIn_Normal;
in vec2 B_FIn_AlbedoUv;
in vec2 B_FIn_NormalMapUv;
in vec3 B_FIn_Tangent;
in mat3 B_TBN;

layout(location = 0) out vec4 B_GIn_Color;
layout(location = 1) out vec4 B_GIn_Albedo;
layout(location = 2) out vec4 B_GIn_Normal;
layout(location = 3) out vec4 B_GIn_Misc;

void main()
{
    vec3 camPosWorld  = B_GetCameraPositionWorld();
    vec3 fragPosWorld = B_FIn_Position;
    vec3 camPosWorldProj  = camPosWorld  * vec3(1,0,1);
    vec3 fragPosWorldProj = fragPosWorld * vec3(1,0,1);
    vec3 camPosProjToFragPosProj = (camPosWorldProj - fragPosWorldProj);
    float sqDist = dot(camPosProjToFragPosProj, camPosProjToFragPosProj);
    float alpha = 1.0f - clamp(sqDist / 50000.0f, 0, 1);

    B_GIn_Misc = vec4(0,0,0,0);
    B_GIn_Normal = vec4(0,1,0,1);
    B_GIn_Color = B_GIn_Albedo = vec4(vec3(1), alpha);
}
