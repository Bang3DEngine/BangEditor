
vec3 HSVToRGB(vec3 hsv)
{
    float h = hsv.r;
    float s = hsv.g;
    float v = hsv.b;

    int   i = int( floor(h * 6) );
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    float newR, newG, newB;
    switch (i % 6)
    {
        case 0: newR = v; newG = t; newB = p; break;
        case 1: newR = q; newG = v; newB = p; break;
        case 2: newR = p; newG = v; newB = t; break;
        case 3: newR = p; newG = q; newB = v; break;
        case 4: newR = t; newG = p; newB = v; break;
        case 5: newR = v; newG = p; newB = q; break;
    }

    return vec3(newR, newG, newB);
}
