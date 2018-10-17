#include "BangEditor/UIInputTexture.h"

#include "Bang/Extensions.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
#include "Bang/Texture2D.h"

namespace Bang
{
class Path;
}

using namespace Bang;
using namespace BangEditor;

UIInputTexture::UIInputTexture()
{
    SetExtensions(Extensions::GetImageExtensions());
}

UIInputTexture::~UIInputTexture()
{
}

RH<Texture2D> UIInputTexture::GetPreviewTextureFromPath(const Path &path)
{
    return Resources::Load<Texture2D>(path);
}
