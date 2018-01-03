#include "BangEditor/ComponentInspectorWidget.h"

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
}

void ComponentInspectorWidget::OnCreateContextMenu(MenuItem *menuRootItem)
{
    menuRootItem->SetFontSize(10);

    MenuItem *remove = menuRootItem->AddItem("Remove");
    menuRootItem->AddSeparator();
    MenuItem *moveUp = menuRootItem->AddItem("Move Up");
    MenuItem *moveDown = menuRootItem->AddItem("Move Down");
    menuRootItem->AddSeparator();
}

