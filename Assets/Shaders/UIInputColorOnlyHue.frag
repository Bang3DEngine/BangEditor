#include "UI.frag"
#include "CommonUIInputColor.glsl"

void main()
{
    vec2 uv = B_FIn_AlbedoUv;
    vec3 hsv = vec3(uv.x, 1, 1);
    vec4 colorRGB = vec4(HSVToRGB(hsv), 1);
    B_GIn_Color = colorRGB;
}
