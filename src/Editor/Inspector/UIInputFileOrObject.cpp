#include "BangEditor/UIInputFileOrObject.h"

#include "Bang/Color.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Paths.h"
#include "Bang/Resources.h"
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
}

UIInputFileOrObject::~UIInputFileOrObject()
{
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
