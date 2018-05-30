#include "BangEditor/UIInputMaterial.h"

#include "Bang/Resources.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/MaterialPreviewFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIInputMaterial::UIInputMaterial()
{
    UIHorizontalLayout *hl = GetComponent<UIHorizontalLayout>();
    GameObject *hlGo = hl->GetGameObject();

    GameObject *previewImgGo = GameObjectFactory::CreateUIGameObject();
    p_previewImg = previewImgGo->AddComponent<UIImageRenderer>();
    p_previewImg->SetTint(Color::White);

    UILayoutElement *previewImgGoLE = previewImgGo->AddComponent<UILayoutElement>();
    previewImgGoLE->SetMinWidth(20);

    previewImgGo->SetParent(hlGo, 0);
}

UIInputMaterial::~UIInputMaterial()
{
}

void UIInputMaterial::SetPath(const Path &path)
{
    UIInputFile::SetPath(path);

    if (GetPath().IsFile())
    {
        RH<Material> material = Resources::Load<Material>(GetPath());
        RH<Texture2D> materialPreviewTex =
                  MaterialPreviewFactory::GetPreviewTextureFor(material.Get());
        p_previewImg->SetImageTexture(materialPreviewTex.Get());
    }
    else
    {
        p_previewImg->SetImageTexture(nullptr);
    }
}


