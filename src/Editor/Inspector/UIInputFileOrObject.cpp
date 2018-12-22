#include "BangEditor/UIInputFileOrObject.h"

#include "Bang/Color.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Paths.h"
#include "Bang/RectTransform.h"
#include "Bang/Assets.h"
#include "Bang/Stretch.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIButton.h"
#include "Bang/UICanvas.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIInputText.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/EditorDialog.h"
#include "BangEditor/EditorTextureFactory.h"

using namespace Bang;
using namespace BangEditor;

UIInputFileOrObject::UIInputFileOrObject()
{
    SetName("UIInputFileOrObject");
    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleWidth(1.0f);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalStretch(Stretch::FULL);
    hl->SetSpacing(5);

    GameObject *previewImgGo = GameObjectFactory::CreateUIGameObject();
    p_previewIcon = previewImgGo->AddComponent<UIImageRenderer>();

    UILayoutElement *previewImgGoLE =
        previewImgGo->AddComponent<UILayoutElement>();
    previewImgGoLE->SetMinWidth(20);

    GameObject *bigPreviewImgGo = GameObjectFactory::CreateUIGameObject();
    bigPreviewImgGo->AddComponent<UILayoutIgnorer>();
    p_bigPreviewIcon = bigPreviewImgGo->AddComponent<UIImageRenderer>();
    bigPreviewImgGo->GetRectTransform()->SetAnchors(Vector2(-1, 1));
    bigPreviewImgGo->GetRectTransform()->SetMarginRight(-128);
    bigPreviewImgGo->GetRectTransform()->SetMarginBot(-128);
    bigPreviewImgGo->GetRectTransform()->TranslateLocal(Vector3(0, 0, -0.1f));

    UIFocusable *previewFocusable = previewImgGo->AddComponent<UIFocusable>();
    previewFocusable->SetConsiderForTabbing(false);
    previewFocusable->EventEmitter<IEventsFocus>::RegisterListener(this);
    bigPreviewImgGo->SetVisible(false);

    p_inputText = GameObjectFactory::CreateUIInputText();
    p_inputText->SetBlocked(true);
    p_inputText->GetText()->SetTextSize(12);
    p_inputText->GetBackground()->SetTint(Color::White());
    UILayoutElement *pathInputTextLE =
        p_inputText->GetGameObject()->AddComponent<UILayoutElement>();
    pathInputTextLE->SetFlexibleSize(Vector2(9999.9f));
    pathInputTextLE->SetLayoutPriority(1);

    Texture2D *lensIcon = EditorTextureFactory::GetLensLittleIcon();
    p_searchButton = GameObjectFactory::CreateUIButton("", lensIcon);
    p_searchButton->SetIcon(lensIcon, Vector2i(16));
    p_searchButton->AddClickedCallback([this]() { OnSearchButtonClicked(); });

    Texture2D *rightArrowIcon = TextureFactory::GetRightArrowIcon();
    p_openButton = GameObjectFactory::CreateUIButton("", rightArrowIcon);
    p_openButton->SetIcon(rightArrowIcon, Vector2i(16));
    p_openButton->GetIcon()->SetTint(Color::Black());
    p_openButton->AddClickedCallback([this]() { OnOpenButtonClicked(); });

    p_inputText->GetGameObject()->SetParent(this);
    p_searchButton->GetGameObject()->SetParent(this);
    p_openButton->GetGameObject()->SetParent(this);
    bigPreviewImgGo->SetParent(this);
    previewImgGo->SetParent(this, 0);

    GetInputText()->GetText()->SetContent("None");
}

UIInputFileOrObject::~UIInputFileOrObject()
{
}

void UIInputFileOrObject::SetZoomable(bool zoomable)
{
    m_zoomable = zoomable;
}

UIInputText *UIInputFileOrObject::GetInputText() const
{
    return p_inputText;
}

UIButton *UIInputFileOrObject::GetSearchButton() const
{
    return p_searchButton;
}

UIButton *UIInputFileOrObject::GetOpenButton() const
{
    return p_openButton;
}

bool UIInputFileOrObject::GetShowPreview() const
{
    return m_showPreview;
}

UIEventResult UIInputFileOrObject::OnUIEvent(UIFocusable *,
                                             const UIEvent &event)
{
    if (event.type == UIEvent::Type::MOUSE_ENTER)
    {
        if (CanDoZoom())
        {
            GetBigPreviewIcon()->GetGameObject()->SetVisible(true);
        }
        return UIEventResult::INTERCEPT;
    }
    else if (event.type == UIEvent::Type::MOUSE_EXIT)
    {
        GetBigPreviewIcon()->GetGameObject()->SetVisible(false);
        return UIEventResult::INTERCEPT;
    }
    return UIEventResult::IGNORE;
}

void UIInputFileOrObject::OnDragStarted(EventEmitter<IEventsDragDrop> *dd_)
{
    IEventsDragDrop::OnDragStarted(dd_);
}

void UIInputFileOrObject::OnDragUpdate(EventEmitter<IEventsDragDrop> *dd_)
{
    IEventsDragDrop::OnDragUpdate(dd_);

    bool acceptedDrag = AcceptsDrag(dd_);

    Color backgroundColor = (acceptedDrag ? Color::Green() : Color::Red());

    UICanvas *canvas = UICanvas::GetActive(this);
    if (acceptedDrag && canvas->IsMouseOver(GetInputText()->GetFocusable()))
    {
        backgroundColor = backgroundColor.WithSaturation(0.3f);
    }
    GetInputText()->GetBackground()->SetTint(backgroundColor);

    Color textColor = (acceptedDrag ? Color::Black() : Color::White());
    GetInputText()->GetText()->SetTextColor(textColor);
}

void UIInputFileOrObject::OnDrop(EventEmitter<IEventsDragDrop> *dd_)
{
    IEventsDragDrop::OnDrop(dd_);

    if (UICanvas *canvas = UICanvas::GetActive(this))
    {
        if (UIDragDroppable *dragDroppable = DCAST<UIDragDroppable *>(dd_))
        {
            if (AcceptsDrag(dd_) &&
                canvas->IsMouseOver(GetInputText()->GetFocusable()))
            {
                OnDropped(dd_);
            }
        }
    }
    GetInputText()->GetBackground()->SetTint(Color::White());
    GetInputText()->GetText()->SetTextColor(Color::Black());
}

void UIInputFileOrObject::SetPreviewIcon(Texture2D *icon, const Color &tint)
{
    GetBigPreviewIcon()->SetImageTexture(icon);
    GetPreviewIcon()->SetImageTexture(icon);
    GetBigPreviewIcon()->SetTint(tint);
    GetPreviewIcon()->SetTint(tint);
}

void UIInputFileOrObject::SetShowPreview(bool showPreview)
{
    m_showPreview = showPreview;
}

UIImageRenderer *UIInputFileOrObject::GetPreviewIcon() const
{
    return p_previewIcon;
}

UIImageRenderer *UIInputFileOrObject::GetBigPreviewIcon() const
{
    return p_bigPreviewIcon;
}

bool UIInputFileOrObject::AcceptsDrag(
    EventEmitter<IEventsDragDrop> *dragDroppable) const
{
    BANG_UNUSED(dragDroppable);
    return true;
}

void UIInputFileOrObject::OnDropped(
    EventEmitter<IEventsDragDrop> *dragDroppable)
{
    BANG_UNUSED(dragDroppable);
}

void UIInputFileOrObject::OnSearchButtonClicked()
{
}

void UIInputFileOrObject::OnOpenButtonClicked()
{
}

bool UIInputFileOrObject::CanDoZoom() const
{
    return m_zoomable;
}
