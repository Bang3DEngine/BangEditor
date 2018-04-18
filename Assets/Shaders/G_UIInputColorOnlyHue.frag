#include "UI.frag"
#include "CommonUIInputColor.glsl"

void main()
{
    vec2 uv = B_FIn_Uv;
    vec3 hsv = vec3(uv.x, 1, 1);
    vec4 colorRGB = vec4(HSVToRGB(hsv), 1);
    B_GIn_Normal = B_GIn_Misc = vec4(0);
    B_GIn_Color = B_GIn_Albedo = colorRGB;
}
