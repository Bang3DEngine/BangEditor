#include "BangEditor/UITabHeader.h"

#include "Bang/Alignment.h"
#include "Bang/Assert.h"
#include "Bang/Cursor.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/IEventsFocus.h"
#include "Bang/TextureFactory.h"
#include "Bang/UICanvas.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/IEventsTabHeader.h"
#include "BangEditor/UITabContainer.h"

using namespace Bang;
using namespace BangEditor;

const Color UITabHeader::ForegroundColor = Color::Zero;
const Color UITabHeader::BackgroundColor = Color::DarkGray.WithValue(1.3f);

UITabHeader::UITabHeader()
{
    GameObjectFactory::CreateUIGameObjectInto(this);
    UIHorizontalLayout *titleHL = AddComponent<UIHorizontalLayout>();
    titleHL->SetPaddings(6);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleHeight(1.0f);
    le->SetLayoutPriority(1);

    p_bg = AddComponent<UIImageRenderer>();

    p_border = AddComponent<UIImageRenderer>();
    p_border->SetImageTexture(TextureFactory::Get9SliceBorder());
    p_border->SetMode(UIImageRenderer::Mode::SLICE_9);
    p_border->SetSlice9BorderStrokePx(Vector2i(1));

    GameObject *titleGo = GameObjectFactory::CreateUIGameObject();
    p_titleText = titleGo->AddComponent<UITextRenderer>();
    p_titleText->SetHorizontalAlign(HorizontalAlignment::LEFT);
    p_titleText->SetVerticalAlign(VerticalAlignment::BOT);
    p_titleText->SetTextSize(12);
    p_titleText->SetContent(GetTitle());

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->AddEventCallback([this](UIFocusable *, const UIEvent &event) {
        if(event.type == UIEvent::Type::MOUSE_CLICK_FULL)
        {
            EventEmitter<IEventsTabHeader>::PropagateToListeners(
                &IEventsTabHeader::OnTabHeaderClicked, this);
            return UIEventResult::INTERCEPT;
        }
        return UIEventResult::IGNORE;
    });
    p_focusable->SetCursorType(Cursor::Type::HAND);

    p_dragDroppable = AddComponent<UIDragDroppable>();
    p_dragDroppable->SetShowDragDropGameObject(true);
    p_dragDroppable->SetFocusable(p_focusable);
    p_dragDroppable->EventEmitter<IEventsDragDrop>::RegisterListener(this);

    titleGo->SetParent(this);

    SetInForeground(false);
}

UITabHeader::~UITabHeader()
{
}

void UITabHeader::Update()
{
    GameObject::Update();

    if(!m_inForeground)
    {
        if(UICanvas *canvas = UICanvas::GetActive(this))
        {
            bool mouseOver = (canvas->IsMouseOver(p_focusable));
            p_bg->SetTint(mouseOver ? m_currentHeaderColor.WithValue(1.5f)
                                    : m_currentHeaderColor);
        }
    }
}

void UITabHeader::SetInForeground(bool inForeground)
{
    m_inForeground = inForeground;
    m_currentHeaderColor = (inForeground ? UITabHeader::ForegroundColor
                                         : UITabHeader::BackgroundColor);
    p_bg->SetTint(m_currentHeaderColor);
    p_border->SetTint(inForeground ? Color::Zero : Color::Black);
}

void UITabHeader::SetTitle(const String &title)
{
    if(title != GetTitle())
    {
        m_title = title;
        p_titleText->SetContent(GetTitle());
    }
}

const String &UITabHeader::GetTitle() const
{
    return m_title;
}

GameObject *UITabHeader::GetTabbedChild() const
{
    return p_tabbedChild;
}

UITabContainer *UITabHeader::GetTabContainer() const
{
    return p_tabContainer;
}

UIDragDroppable *UITabHeader::GetDragDroppable() const
{
    return p_dragDroppable;
}

void UITabHeader::SetTabContainer(UITabContainer *tabContainer)
{
    p_tabContainer = tabContainer;
    if(GetTabContainer())
    {
        p_lastTabContainer = GetTabContainer();
        ASSERT(!GetTabContainer()->IsWaitingToBeDestroyed())
    }
}

void UITabHeader::SetTabbedChild(GameObject *tabbedChild)
{
    p_tabbedChild = tabbedChild;
}

void UITabHeader::OnDragStarted(EventEmitter<IEventsDragDrop> *dragDropEmitter)
{
    BANG_UNUSED(dragDropEmitter);

    if(dragDropEmitter == GetDragDroppable())
    {
        ASSERT(GetTabbedChild());
        p_dragTabContainerDestiny = nullptr;
    }
}

void UITabHeader::OnDragUpdate(EventEmitter<IEventsDragDrop> *dragDropEmitter)
{
    BANG_UNUSED(dragDropEmitter);
}

void UITabHeader::OnDrop(EventEmitter<IEventsDragDrop> *dragDropEmitter)
{
    if(dragDropEmitter == GetDragDroppable())
    {
        if(GetTabContainer())
        {
            GetTabContainer()->RemoveTabByHeader(this, false);
        }

        UITabContainer *destinyTabContainer = p_dragTabContainerDestiny
                                                  ? p_dragTabContainerDestiny
                                                  : p_lastTabContainer;
        if(destinyTabContainer)
        {
            destinyTabContainer->AddTabByTabHeader(
                this, m_dragTabContainerDestinyIndex);
        }
    }
}
