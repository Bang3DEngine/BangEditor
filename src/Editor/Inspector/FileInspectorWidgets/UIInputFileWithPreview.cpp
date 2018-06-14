#include "BangEditor/UIInputFileWithPreview.h"

#include "Bang/UICanvas.h"
#include "Bang/Resources.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIInputFileWithPreview::UIInputFileWithPreview()
{
    UIHorizontalLayout *hl = GetComponent<UIHorizontalLayout>();
    GameObject *hlGo = hl->GetGameObject();

    GameObject *previewImgGo = GameObjectFactory::CreateUIGameObject();
    p_previewImg = previewImgGo->AddComponent<UIImageRenderer>();

    UILayoutElement *previewImgGoLE = previewImgGo->AddComponent<UILayoutElement>();
    previewImgGoLE->SetMinWidth(20);

    GameObject *bigPreviewImgGo = GameObjectFactory::CreateUIGameObject();
    bigPreviewImgGo->AddComponent<UILayoutIgnorer>();
    p_bigPreviewImg = bigPreviewImgGo->AddComponent<UIImageRenderer>();
    bigPreviewImgGo->GetRectTransform()->SetAnchors( Vector2(-1, 1) );
    bigPreviewImgGo->GetRectTransform()->SetMarginRight(-128);
    bigPreviewImgGo->GetRectTransform()->SetMarginBot(-128);
    bigPreviewImgGo->GetRectTransform()->TranslateLocal( Vector3(0, 0, -0.1f) );

    UIFocusable *previewFocusable = previewImgGo->AddComponent<UIFocusable>();
    previewFocusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    bigPreviewImgGo->SetParent(hlGo);
    bigPreviewImgGo->SetVisible(false);

    previewImgGo->SetParent(hlGo, 0);
}

UIInputFileWithPreview::~UIInputFileWithPreview()
{
}

void UIInputFileWithPreview::SetPath(const Path &path)
{
    UIInputFile::SetPath(path);

    bool isGoodPath = (path.IsFile() || Resources::IsEmbeddedResource(path));
    RH<Texture2D> previewTex;
    if (isGoodPath)
    {
        previewTex = GetPreviewTextureFromPath(path);
    }

    if (previewTex)
    {
        p_bigPreviewImg->SetImageTexture( previewTex.Get() );
        p_previewImg->SetImageTexture( previewTex.Get() );
    }
    else
    {
        p_bigPreviewImg->SetImageTexture(nullptr);
        p_previewImg->SetImageTexture(nullptr);
    }
}

void UIInputFileWithPreview::OnMouseEnter(EventEmitter<IEventsFocus> *focusable)
{
    IEventsFocus::OnMouseEnter(focusable);
    p_bigPreviewImg->GetGameObject()->SetVisible(true);
}

void UIInputFileWithPreview::OnMouseExit(EventEmitter<IEventsFocus> *focusable)
{
    IEventsFocus::OnMouseExit(focusable);
    p_bigPreviewImg->GetGameObject()->SetVisible(false);
}
