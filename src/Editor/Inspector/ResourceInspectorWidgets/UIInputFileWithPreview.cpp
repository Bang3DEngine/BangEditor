#include "BangEditor/UIInputFileWithPreview.h"

#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Path.h"
#include "Bang/RectTransform.h"
#include "Bang/Resource.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "BangEditor/EditorTextureFactory.h"

using namespace Bang;
using namespace BangEditor;

UIInputFileWithPreview::UIInputFileWithPreview()
{
    UIHorizontalLayout *hl = GetComponent<UIHorizontalLayout>();
    GameObject *hlGo = hl->GetGameObject();

    GameObject *previewImgGo = GameObjectFactory::CreateUIGameObject();
    p_previewImg = previewImgGo->AddComponent<UIImageRenderer>();

    UILayoutElement *previewImgGoLE =
        previewImgGo->AddComponent<UILayoutElement>();
    previewImgGoLE->SetMinWidth(20);

    GameObject *bigPreviewImgGo = GameObjectFactory::CreateUIGameObject();
    bigPreviewImgGo->AddComponent<UILayoutIgnorer>();
    p_bigPreviewImg = bigPreviewImgGo->AddComponent<UIImageRenderer>();
    bigPreviewImgGo->GetRectTransform()->SetAnchors(Vector2(-1, 1));
    bigPreviewImgGo->GetRectTransform()->SetMarginRight(-128);
    bigPreviewImgGo->GetRectTransform()->SetMarginBot(-128);
    bigPreviewImgGo->GetRectTransform()->TranslateLocal(Vector3(0, 0, -0.1f));

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

void UIInputFileWithPreview::SetResource(Resource *resource)
{
    if(resource)
    {
        SetPath(resource->GetResourceFilepath());
    }
    else
    {
        SetPath(Path::Empty);
    }
}

void UIInputFileWithPreview::SetPath(const Path &path)
{
    UIInputFile::SetPath(path);

    RH<Texture2D> previewTex;
    if(HasExistingPath())
    {
        previewTex = GetPreviewTextureFromPath(path);
    }
    p_bigPreviewImg->SetImageTexture(previewTex.Get());
    p_previewImg->SetImageTexture(previewTex.Get());
}

RH<Texture2D> UIInputFileWithPreview::GetPreviewTextureFromPath(
    const Path &path)
{
    return RH<Texture2D>(EditorTextureFactory::GetIconForPath(path));
}

bool UIInputFileWithPreview::HasExistingPath() const
{
    return (GetPath().IsFile() || Resources::IsEmbeddedResource(GetPath()));
}

UIEventResult UIInputFileWithPreview::OnUIEvent(UIFocusable *,
                                                const UIEvent &event)
{
    if(event.type == UIEvent::Type::MOUSE_ENTER)
    {
        if(HasExistingPath() && m_zoomable)
        {
            p_bigPreviewImg->GetGameObject()->SetVisible(true);
        }
        return UIEventResult::INTERCEPT;
    }
    else if(event.type == UIEvent::Type::MOUSE_EXIT)
    {
        p_bigPreviewImg->GetGameObject()->SetVisible(false);
        return UIEventResult::INTERCEPT;
    }
    return UIEventResult::IGNORE;
}
