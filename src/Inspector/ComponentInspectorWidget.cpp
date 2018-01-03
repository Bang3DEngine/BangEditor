#include "BangEditor/ComponentInspectorWidget.h"

#include "Bang/Debug.h"
#include "Bang/Component.h"
#include "Bang/UIFocusable.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ComponentInspectorWidget::ComponentInspectorWidget()
{
    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback([this](MenuItem *menuRootItem)
    {
        OnCreateContextMenu(menuRootItem);
    });
    AddComponent<UIFocusable>();
    p_contextMenu->AddButtonPart( this );
    p_contextMenu->AddButtonPart( GetWidgetsContainer() );
}

ComponentInspectorWidget::~ComponentInspectorWidget()
{
}

void ComponentInspectorWidget::SetComponent(Component *comp)
{
    SetTitle(comp->GetClassName());
    p_component = comp;
}

Component *ComponentInspectorWidget::GetComponent() const
{
    return p_component;
}

void ComponentInspectorWidget::OnCreateContextMenu(MenuItem *menuRootItem)
{
    menuRootItem->SetFontSize(10);

    MenuItem *remove = menuRootItem->AddItem("Remove");
    remove->SetSelectedCallback([this](MenuItem*)
    {
        GameObject *go = GetComponent()->GetGameObject();
        go->RemoveComponent(GetComponent());
    });

    menuRootItem->AddSeparator();
    MenuItem *moveUp = menuRootItem->AddItem("Move Up");
    moveUp->SetSelectedCallback([this](MenuItem*)
    {
        MoveComponent(GetComponent(), -1);
    });

    MenuItem *moveDown = menuRootItem->AddItem("Move Down");
    moveDown->SetSelectedCallback([this](MenuItem*)
    {
        MoveComponent(GetComponent(), 1);
    });
    menuRootItem->AddSeparator();
}

void ComponentInspectorWidget::MoveComponent(Component *comp, int offset)
{
    GameObject *go = comp->GetGameObject();
    int prevIndex = go->GetComponents().IndexOf(comp);
    const int numComps = go->GetComponents().Size();
    int newIndex = (prevIndex + offset + numComps) % numComps;
    Debug_Log("Moving to " << newIndex);
    go->RemoveComponent(comp);
    go->AddComponent(comp, newIndex);
}

