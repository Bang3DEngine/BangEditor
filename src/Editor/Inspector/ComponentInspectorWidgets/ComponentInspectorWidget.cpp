#include "BangEditor/ComponentInspectorWidget.h"

#include "Bang/Debug.h"
#include "Bang/Component.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIFocusable.h"
#include "Bang/UITextRenderer.h"
#include "Bang/MaterialFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoSerializableChange.h"

#include "BangEditor/EditorClipboard.h"
#include "BangEditor/EditorTextureFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ComponentInspectorWidget::ComponentInspectorWidget()
{
}

ComponentInspectorWidget::~ComponentInspectorWidget()
{
}

void ComponentInspectorWidget::InitInnerWidgets()
{
    InspectorWidget::InitInnerWidgets();

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback([this](MenuItem *menuRootItem)
    {
        OnCreateContextMenu(menuRootItem);
    });
    AddComponent<UIFocusable>();
    p_contextMenu->AddButtonPart( this );
    p_contextMenu->AddButtonPart( GetWidgetsContainer() );

    GetInspectorWidgetTitle()->GetEnabledCheckBox()->SetEnabled(
                MustShowEnabledCheckbox() );
    GetInspectorWidgetTitle()->GetEnabledCheckBox()->
                    EventEmitter<IEventsValueChanged>::RegisterListener(this);
}

void ComponentInspectorWidget::UpdateFromReference()
{
    GetInspectorWidgetTitle()->GetEnabledCheckBox()->
                               SetChecked( GetComponent()->IsEnabled() );
}

void ComponentInspectorWidget::SetComponent(Component *comp)
{
    ASSERT(!GetComponent());

    p_component = comp;
    UIImageRenderer *icon = GetInspectorWidgetTitle()->GetIcon();
    icon->SetImageTexture( GetComponentIconTexture() );
    icon->SetTint( GetComponentIconTint() );
    Update();
}

Component *ComponentInspectorWidget::GetComponent() const
{
    return p_component;
}

GameObject *ComponentInspectorWidget::GetInspectedGameObject() const
{
    return GetComponent()->GetGameObject();
}

void ComponentInspectorWidget::SetTitle(const String &title)
{
    GetInspectorWidgetTitle()->GetText()->SetContent(title);
}

bool ComponentInspectorWidget::CanBeRemovedFromContextMenu() const
{
    return true;
}

void ComponentInspectorWidget::OnValueChangedCIW(
                            EventEmitter<IEventsValueChanged> *object)
{
    if (object == GetInspectorWidgetTitle()->GetEnabledCheckBox())
    {
        GetComponent()->SetEnabled( GetInspectorWidgetTitle()->
                                    GetEnabledCheckBox()->IsChecked() );
    }
}


void ComponentInspectorWidget::OnValueChanged(
                            EventEmitter<IEventsValueChanged> *object)
{
    XMLNode undoXMLBefore;
    if (GetComponent())
    {
        undoXMLBefore = GetComponent()->GetXMLInfo();
    }

    OnValueChangedCIW(object);

    if (GetComponent())
    {
        PushCurrentStateToUndoRedoIfAnyChangeForComponent(undoXMLBefore);
    }
}

Texture2D* ComponentInspectorWidget::GetComponentIconTexture() const
{
    if (GetComponent())
    {
        String componentName = GetComponent()->GetClassName();
        return EditorTextureFactory::GetComponentIcon(componentName);
    }
    return EditorTextureFactory::GetCubeIcon();
}

Color ComponentInspectorWidget::GetComponentIconTint() const
{
    if (GetComponent())
    {
        String componentName = GetComponent()->GetClassName();
        return EditorTextureFactory::GetComponentIconTint(componentName);
    }
    return Color::White;
}

void ComponentInspectorWidget::PushCurrentStateToUndoRedoIfAnyChangeForComponent(
                                                const XMLNode &undoXMLBefore)
{
    XMLNode currentXML = GetComponent()->GetXMLInfo();
    if (currentXML.ToString() != undoXMLBefore.ToString())
    {
        UndoRedoManager::PushAction( new UndoRedoSerializableChange(GetComponent(),
                                                                    undoXMLBefore,
                                                                    currentXML) );
    }
}

void ComponentInspectorWidget::PushCurrentStateToUndoRedoIfAnyChangeForGameObject(
                                                const XMLNode &undoXMLBefore)
{
    GameObject *go = GetComponent()->GetGameObject();
    XMLNode currentXML = go->GetXMLInfo();
    if (currentXML.ToString() != undoXMLBefore.ToString())
    {
        UndoRedoManager::PushAction(
                new UndoRedoSerializableChange(go, undoXMLBefore, currentXML) );
    }
}

void ComponentInspectorWidget::OnCreateContextMenu(MenuItem *menuRootItem)
{
    menuRootItem->SetFontSize(12);

    MenuItem *remove = menuRootItem->AddItem("Remove");
    remove->SetSelectedCallback([this](MenuItem*)
    {
        XMLNode undoXMLBefore = GetComponent()->GetGameObject()->GetXMLInfo();
        GameObject *go = GetComponent()->GetGameObject();
        go->RemoveComponent(GetComponent());
        PushCurrentStateToUndoRedoIfAnyChangeForGameObject(undoXMLBefore);
    });
    menuRootItem->AddSeparator();

    MenuItem *copy = menuRootItem->AddItem("Copy");
    copy->SetSelectedCallback([this](MenuItem*)
    {
        EditorClipboard::CopyComponent( GetComponent() );
    });

    MenuItem *cut = menuRootItem->AddItem("Cut");
    cut->SetSelectedCallback([this](MenuItem*)
    {
        XMLNode undoXMLBefore = GetComponent()->GetGameObject()->GetXMLInfo();
        EditorClipboard::CopyComponent( GetComponent() );
        PushCurrentStateToUndoRedoIfAnyChangeForGameObject(undoXMLBefore);
        Component::Destroy( GetComponent() );
    });

    MenuItem *paste = menuRootItem->AddItem("Paste");
    paste->SetSelectedCallback([this](MenuItem*)
    {
        XMLNode undoXMLBefore = GetComponent()->GetGameObject()->GetXMLInfo();
        Component *copiedComp = EditorClipboard::GetCopiedComponent();
        Component *newComponent = copiedComp->Clone();
        GetInspectedGameObject()->AddComponent(newComponent);
        PushCurrentStateToUndoRedoIfAnyChangeForGameObject(undoXMLBefore);
    });
    paste->SetOverAndActionEnabled( (EditorClipboard::HasCopiedComponent()) );

    MenuItem *pasteValues = menuRootItem->AddItem("Paste values");
    pasteValues->SetSelectedCallback([this](MenuItem*)
    {
        XMLNode undoXMLBefore = GetComponent()->GetXMLInfo();
        Component *copiedComp = EditorClipboard::GetCopiedComponent();
        copiedComp->CloneInto( GetComponent() );
        PushCurrentStateToUndoRedoIfAnyChangeForComponent(undoXMLBefore);
    });
    pasteValues->SetOverAndActionEnabled( (EditorClipboard::HasCopiedComponent()) );

    menuRootItem->AddSeparator();

    MenuItem *moveUp = menuRootItem->AddItem("Move Up");
    moveUp->SetSelectedCallback([this](MenuItem*)
    {
        XMLNode undoXMLBefore = GetComponent()->GetGameObject()->GetXMLInfo();
        MoveComponent(GetComponent(), -1);
        PushCurrentStateToUndoRedoIfAnyChangeForGameObject(undoXMLBefore);
    });

    MenuItem *moveDown = menuRootItem->AddItem("Move Down");
    moveDown->SetSelectedCallback([this](MenuItem*)
    {
        XMLNode undoXMLBefore = GetComponent()->GetGameObject()->GetXMLInfo();
        MoveComponent(GetComponent(), 1);
        PushCurrentStateToUndoRedoIfAnyChangeForGameObject(undoXMLBefore);
    });

    remove->SetOverAndActionEnabled( CanBeRemovedFromContextMenu() );
    cut->SetOverAndActionEnabled( CanBeRemovedFromContextMenu() );
}

void ComponentInspectorWidget::MoveComponent(Component *comp, int offset)
{
    GameObject *go = comp->GetGameObject();
    int prevIndex = go->GetComponents().IndexOf(comp);
    const int numComps = go->GetComponents().Size();
    int newIndex = (prevIndex + offset + numComps) % numComps;
    go->RemoveComponent(comp);
    go->AddComponent(comp, newIndex);
}

bool ComponentInspectorWidget::MustShowEnabledCheckbox() const
{
    return true;
}
