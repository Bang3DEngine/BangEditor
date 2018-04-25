#include "UI.frag"
#include "CommonUIInputColor.glsl"

uniform float B_Hue;

void main()
{
    vec2 uv = B_FIn_AlbedoUv;
    vec3 hsv = vec3(B_Hue, uv.x, 1.0-uv.y);
    vec4 colorRGB = vec4(HSVToRGB(hsv), 1);
    B_GIn_Color = colorRGB;
}
