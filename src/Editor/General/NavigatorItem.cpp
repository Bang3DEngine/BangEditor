#include "BangEditor/NavigatorItem.h"

#include "Bang/Alignment.h"
#include "Bang/AspectRatioMode.h"
#include "Bang/Color.h"
#include "Bang/DPtr.tcc"
#include "Bang/File.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/Key.h"
#include "Bang/MetaFilesManager.h"
#include "Bang/RectTransform.h"
#include "Bang/Texture2D.h"
#include "Bang/UIAspectRatioFitter.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILabel.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UITheme.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/MenuItem.h"

using namespace Bang;
using namespace BangEditor;

NavigatorItem::NavigatorItem()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetPreferredSize(Vector2i(100));

    constexpr int textPixels = 30;
    constexpr int spacing = 5;

    GameObject *bgGo = GameObjectFactory::CreateUIGameObject();
    p_bg = bgGo->AddComponent<UIImageRenderer>();

    GameObject *iconContainerGo = GameObjectFactory::CreateUIGameObject();
    RectTransform *iconContainerRT = iconContainerGo->GetRectTransform();
    iconContainerRT->SetMarginBot(textPixels + spacing);

    GameObject *iconGo = GameObjectFactory::CreateUIGameObject();
    RectTransform *iconRT = iconGo->GetRectTransform();
    iconRT->SetAnchors(Vector2::Zero());
    iconRT->SetPivotPosition(Vector2::Zero());
    p_icon = iconGo->AddComponent<UIImageRenderer>();
    p_icon->SetTint(Color::Zero());

    p_aspectRatioFitter = iconGo->AddComponent<UIAspectRatioFitter>();
    p_aspectRatioFitter->SetAspectRatioMode(AspectRatioMode::KEEP);

    p_label = GameObjectFactory::CreateUILabel();
    GameObject *labelGo = p_label->GetGameObject();
    RectTransform *labelRT = labelGo->GetRectTransform();
    labelRT->SetAnchorX(Vector2(-1, 1));
    labelRT->SetAnchorY(Vector2(-1, -1));
    labelRT->SetMarginTop(-textPixels);
    p_label->GetText()->SetTextSize(11);
    p_label->GetText()->SetContent("");
    p_label->GetText()->SetWrapping(true);
    p_label->GetFocusable()->SetEnabled(false);
    p_label->GetText()->SetVerticalAlign(VerticalAlignment::TOP);
    p_label->GetText()->SetHorizontalAlign(HorizontalAlignment::CENTER);
    p_label->SetSelectable(false);

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->SetConsiderForTabbing(true);
    GetFocusable()->EventEmitter<IEventsFocus>::RegisterListener(this);

    bgGo->SetParent(this);
    labelGo->SetParent(this);
    iconContainerGo->SetParent(this);
    iconGo->SetParent(iconContainerGo);

    SetSelected(false);
}

NavigatorItem::~NavigatorItem()
{
}

UIEventResult NavigatorItem::OnUIEvent(UIFocusable *focusable,
                                       const UIEvent &event)
{
    switch (event.type)
    {
        case UIEvent::Type::FOCUS_TAKEN:
            SetSelected(true);
            OnFocusTaken(focusable, event);
            return UIEventResult::INTERCEPT;

        case UIEvent::Type::FOCUS_LOST:
            SetSelected(false);
            return UIEventResult::INTERCEPT;

        case UIEvent::Type::MOUSE_ENTER:
            if (!IsSelected() && p_bg)
            {
                p_bg->SetTint(UITheme::GetOverColor());
            }
            return UIEventResult::INTERCEPT;

        case UIEvent::Type::MOUSE_EXIT:
            if (!IsSelected() && p_bg)
            {
                p_bg->SetTint(Color::Zero());
            }
            return UIEventResult::INTERCEPT;

        default: break;
    }
    return UIEventResult::IGNORE;
}

void NavigatorItem::OnFocusTaken(UIFocusable *, const UIEvent &)
{
    // Empty
}

void NavigatorItem::SetSelected(bool selected)
{
    m_selected = selected;

    if (p_bg)
    {
        p_bg->SetTint(IsSelected() ? UITheme::GetSelectedColor()
                                   : Color::Zero());
    }
}

bool NavigatorItem::IsSelected() const
{
    return m_selected;
}

UILabel *NavigatorItem::GetLabel() const
{
    return p_label;
}

UIImageRenderer *NavigatorItem::GetIcon() const
{
    return p_icon;
}

UIAspectRatioFitter *NavigatorItem::GetAspectRatioFitter() const
{
    return p_aspectRatioFitter;
}

UIFocusable *NavigatorItem::GetFocusable() const
{
    return p_focusable;
}
