#include "UI.frag"

uniform int B_SceneRenderMode;

const int COLOR            = 0;
const int NORMAL           = 1;
const int ALBEDO           = 2;
const int DEPTH            = 3;
const int ROUGHNESS        = 4;
const int METALNESS        = 5;
const int RECEIVES_LIGHT   = 6;
const int RECEIVES_SHADOWS = 7;
const int SELECTION        = 8;
const int WORLD_POSITION   = 9;

void main()
{
    vec3 color = vec3(1,0,1);
    switch (B_SceneRenderMode)
    {
        case COLOR:
            color = B_SampleColor(B_FIn_AlbedoUv).rgb;
        break;

        case NORMAL:
            color = B_SampleNormal(B_FIn_AlbedoUv).rgb;
        break;

        case ALBEDO:
            color = B_SampleAlbedoColor(B_FIn_AlbedoUv).rgb;
        break;

        case DEPTH:
            color = vec3(1,0,1);
        break;

        case WORLD_POSITION:
            color = B_ComputeWorldPosition();
        break;

        case ROUGHNESS:
            color = vec3( B_SampleRoughness(B_FIn_AlbedoUv) );
        break;

        case METALNESS:
            color = vec3( B_SampleMetalness(B_FIn_AlbedoUv) );
        break;

        case RECEIVES_LIGHT:
            color = vec3( B_SampleReceivesLight(B_FIn_AlbedoUv) );
        break;

        case RECEIVES_SHADOWS:
            color = vec3( B_SampleReceivesShadows(B_FIn_AlbedoUv) );
        break;

        case SELECTION:
            color = vec3(1,0,1);
        break;
    }

    B_GIn_Color = vec4(color, 1);
}
