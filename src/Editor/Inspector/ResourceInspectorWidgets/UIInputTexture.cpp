#include "BangEditor/UIInputTexture.h"

#include "Bang/Extensions.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
#include "Bang/Texture2D.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Path;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIInputTexture::UIInputTexture()
{
    SetExtensions( Extensions::GetImageExtensions() );
}

UIInputTexture::~UIInputTexture()
{
}

RH<Texture2D> UIInputTexture::GetPreviewTextureFromPath(const Path &path)
{
    return Resources::Load<Texture2D>(path);
}
