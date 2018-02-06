#include "BangEditor/UIContextMenu.h"

#include "Bang/Input.h"
#include "Bang/UIList.h"
#include "Bang/UICanvas.h"
#include "Bang/IFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

#include "BangEditor/MenuItem.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

// UIContextMenu
void UIContextMenu::OnUpdate()
{
    Component::OnUpdate();

    if (Input::GetMouseButtonDown(MouseButton::Right))
    {
        List<GameObject*> parts = m_parts;
        parts.PushBack(GetGameObject());

        for (GameObject *part : m_parts)
        {
            if (UICanvas::GetActive(this)->IsMouseOver(part, true))
            {
                ShowMenu();
                break;
            }
        }
    }
}

void UIContextMenu::ShowMenu()
{
    if (p_menu)
    {
        GameObject::Destroy(p_menu);
        p_menu = nullptr;
    }

    // if (!IsMenuBeingShown())
    {
        p_menu = GameObject::Create<ContextMenu>();
        if (m_createContextMenuCallback)
        {
            m_createContextMenuCallback(p_menu->GetRootItem());
        }
        p_menu->EventEmitter<IDestroyListener>::RegisterListener(this);
        p_menu->SetParent( EditorSceneManager::GetEditorScene() );
    }
}

bool UIContextMenu::IsMenuBeingShown() const
{
    return p_menu != nullptr;
}

void UIContextMenu::AddButtonPart(GameObject *part)
{
    m_parts.PushBack(part);
}

void UIContextMenu::SetCreateContextMenuCallback(
        UIContextMenu::CreateContextMenuCallback createCallback)
{
    m_createContextMenuCallback = createCallback;
}

void UIContextMenu::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    ASSERT(p_menu && object == p_menu);
    p_menu = nullptr;
}

// ContextMenu
ContextMenu::ContextMenu()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    p_rootItem = GameObject::Create<MenuItem>( MenuItem::MenuItemType::Root );
    GetRootItem()->SetDestroyOnClose(true);

    RectTransform *rt = GetRootItem()->GetRectTransform();
    Vector2 mousePosNDC = Input::GetMousePositionNDC();
    rt->SetAnchors( mousePosNDC );
    rt->TranslateLocal( Vector3(0, 0, -0.001f) );
    rt->SetPivotPosition( Vector2(-1, 1) );
    AdjustToBeInsideScreen();

    UIContentSizeFitter *csf = GetRootItem()->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);

    GetRootItem()->AddComponent<UILayoutIgnorer>();
    GetRootItem()->EventEmitter<IDestroyListener>::RegisterListener(this);
    GetRootItem()->SetDestroyOnClose(true);
    GetRootItem()->SetParent(this);

    m_justCreated = true;
}

void ContextMenu::AdjustToBeInsideScreen()
{
    RectTransform *rt = GetRootItem()->GetRectTransform();
    RectTransform *listRT = GetRootItem()->GetChildrenList()->GetGameObject()->
                            GetRectTransform();

    Vector2 pivotPos = listRT->GetPivotPosition();

    AARect allRect ( GetRootItem()->GetRectTransform()->GetViewportRectNDC() );
    List<GameObject*> menuChildren = GetRootItem()->GetChildrenRecursively();
    for (GameObject *child : menuChildren)
    {
        RectTransform *crt = child->GetRectTransform();
        if (crt) { allRect = AARect::Union(allRect,
                                           AARect( crt->GetViewportRectNDC() )); }
    }

    for (int axis = 0; axis < 2; ++axis) // X and Y
    {
        // Avoid side where it overflows.
        // If it overflows both, just pick 1
        bool overflowingMax = ((rt->GetAnchorMax() + allRect.GetSize())[axis] >  1.0f);
        bool overflowingMin = ((rt->GetAnchorMin() - allRect.GetSize())[axis] < -1.0f);
        if      (overflowingMax && overflowingMin) { pivotPos[axis] =  1.0f; }
        else if (overflowingMax)                   { pivotPos[axis] =  1.0f; }
        else if (overflowingMin)                   { pivotPos[axis] = -1.0f; }
    }

    listRT->SetPivotPosition(pivotPos);
}

void ContextMenu::Update()
{
    GameObject::Update();

    if (Input::GetMouseButtonDown(MouseButton::Right) ||
        Input::GetMouseButtonDown(MouseButton::Left))
    {
        if (!m_justCreated &&
            !GetRootItem()->GetRectTransform()->IsMouseOver(true))
        {
            GameObject::Destroy(this);
        }
    }
    m_justCreated = false;
    AdjustToBeInsideScreen();
}

MenuItem *ContextMenu::GetRootItem() const
{
    return p_rootItem;
}

void ContextMenu::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    ASSERT(object == p_rootItem);
    GameObject::Destroy(this);
}
