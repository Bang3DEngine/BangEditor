#include "BangEditor/UIInputTexture.h"

#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIInputTexture::UIInputTexture()
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

UIInputTexture::~UIInputTexture()
{
}

void UIInputTexture::SetPath(const Path &path)
{
    UIInputFile::SetPath(path);

    if (GetPath().IsFile())
    {
        p_previewImg->SetImageTexture(GetPath());
    }
    else
    {
        p_previewImg->SetImageTexture(nullptr);
    }
}

