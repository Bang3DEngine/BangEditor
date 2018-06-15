#include "BangEditor/UIInputMaterial.h"

#include "Bang/Material.h"
#include "Bang/Resources.h"

#include "BangEditor/MaterialPreviewFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIInputMaterial::UIInputMaterial()
{
}

UIInputMaterial::~UIInputMaterial()
{
}

RH<Texture2D> UIInputMaterial::GetPreviewTextureFromPath(const Path &path)
{
    RH<Material> material = Resources::Load<Material>(path);
    RH<Texture2D> materialPreviewTex =
              MaterialPreviewFactory::GetPreviewTextureFor(material.Get());
    return materialPreviewTex;
}
