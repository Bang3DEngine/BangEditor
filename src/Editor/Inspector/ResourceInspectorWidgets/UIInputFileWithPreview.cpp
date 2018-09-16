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

#include "BangEditor/EditorTextureFactory.h"

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
    previewFocusable->SetConsiderForTabbing(false);
    previewFocusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    bigPreviewImgGo->SetParent(hlGo);
    bigPreviewImgGo->SetVisible(false);

    previewImgGo->SetParent(hlGo, 0);
}

UIInputFileWithPreview::~UIInputFileWithPreview()
{
}

void UIInputFileWithPreview::SetZoomable(bool zoomable)
{
    m_zoomable = zoomable;
}

void UIInputFileWithPreview::SetPath(const Path &path)
{
    UIInputFile::SetPath(path);

    RH<Texture2D> previewTex;
    if ( HaveExistingPath() )
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

RH<Texture2D> UIInputFileWithPreview::GetPreviewTextureFromPath(const Path &path)
{
    return RH<Texture2D>( EditorTextureFactory::GetIconForPath(path) );
}

bool UIInputFileWithPreview::HaveExistingPath() const
{
    return (GetPath().IsFile() || Resources::IsEmbeddedResource( GetPath() ));
}

UIEventResult UIInputFileWithPreview::OnUIEvent(UIFocusable*,
                                                const UIEvent &event)
{
    if (event.type == UIEvent::Type::MOUSE_ENTER)
    {
        if (HaveExistingPath() && m_zoomable)
        {
            p_bigPreviewImg->GetGameObject()->SetVisible(true);
        }
        return UIEventResult::INTERCEPT;
    }
    else if (event.type == UIEvent::Type::MOUSE_EXIT)
    {
        p_bigPreviewImg->GetGameObject()->SetVisible(false);
        return UIEventResult::INTERCEPT;
    }
    return UIEventResult::IGNORE;
}
