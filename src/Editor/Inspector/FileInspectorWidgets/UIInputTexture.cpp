#include "BangEditor/UIInputTexture.h"

#include "Bang/Resources.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIInputTexture::UIInputTexture()
{
}

UIInputTexture::~UIInputTexture()
{
}

RH<Texture2D> UIInputTexture::GetPreviewTextureFromPath(const Path &path)
{
    return Resources::Load<Texture2D>(path);
}
