#include "UI.frag"

uniform mat4 B_Rotation;
uniform samplerCube B_TextureCubeMapToPreview;

void main()
{
    vec2 uv = B_FIn_AlbedoUv;
    uv.y = 1.0 - uv.y;
    vec3 forward = vec3(uv, 1);
    forward = (B_Rotation * vec4(forward, 0)).xyz;

    vec3 texCMColor = texture(B_TextureCubeMapToPreview, forward).rgb;
    B_GIn_Color = vec4(texCMColor, 1);
}
