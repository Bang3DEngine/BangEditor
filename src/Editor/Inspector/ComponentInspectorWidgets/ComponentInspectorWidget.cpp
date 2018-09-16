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

    UIFocusable *focusable = AddComponent<UIFocusable>();
    p_contextMenu->SetFocusable(focusable);

    GetInspectorWidgetTitle()->GetEnabledCheckBox()->
                    GetGameObject()->SetEnabled( MustShowEnabledCheckbox() );
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
    OnComponentSet();
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
    MetaNode undoMetaBefore;
    if (GetComponent())
    {
        undoMetaBefore = GetComponent()->GetMeta();
    }

    OnValueChangedCIW(object);

    if (GetComponent())
    {
        PushCurrentStateToUndoRedo(undoMetaBefore);
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

void ComponentInspectorWidget::PushCurrentStateToUndoRedo(
                                                const MetaNode &undoMetaBefore)
{
    MetaNode currentMeta = GetComponent()->GetMeta();
    UndoRedoManager::PushAction( new UndoRedoSerializableChange(GetComponent(),
                                                                undoMetaBefore,
                                                                currentMeta) );
}

void ComponentInspectorWidget::PushCurrentStateToUndoRedoIfAnyChangeForGameObject(
                                                const MetaNode &undoMetaBefore)
{
    GameObject *go = GetComponent()->GetGameObject();
    MetaNode currentMeta = go->GetMeta();
    if (currentMeta.ToString() != undoMetaBefore.ToString())
    {
        UndoRedoManager::PushAction(
                new UndoRedoSerializableChange(go, undoMetaBefore, currentMeta) );
    }
}

void ComponentInspectorWidget::OnCreateContextMenu(MenuItem *menuRootItem)
{
    menuRootItem->SetFontSize(12);

    MenuItem *remove = menuRootItem->AddItem("Remove");
    remove->SetSelectedCallback([this](MenuItem*)
    {
        MetaNode undoMetaBefore = GetComponent()->GetGameObject()->GetMeta();
        GameObject *go = GetComponent()->GetGameObject();
        go->RemoveComponent(GetComponent());
        PushCurrentStateToUndoRedoIfAnyChangeForGameObject(undoMetaBefore);
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
        MetaNode undoMetaBefore = GetComponent()->GetGameObject()->GetMeta();
        EditorClipboard::CopyComponent( GetComponent() );
        PushCurrentStateToUndoRedoIfAnyChangeForGameObject(undoMetaBefore);
        Component::Destroy( GetComponent() );
    });

    MenuItem *paste = menuRootItem->AddItem("Paste");
    paste->SetSelectedCallback([this](MenuItem*)
    {
        MetaNode undoMetaBefore = GetComponent()->GetGameObject()->GetMeta();
        Component *copiedComp = EditorClipboard::GetCopiedComponent();
        Component *newComponent = copiedComp->Clone();
        GetInspectedGameObject()->AddComponent(newComponent);
        PushCurrentStateToUndoRedoIfAnyChangeForGameObject(undoMetaBefore);
    });
    paste->SetOverAndActionEnabled( (EditorClipboard::HasCopiedComponent()) );

    MenuItem *pasteValues = menuRootItem->AddItem("Paste values");
    pasteValues->SetSelectedCallback([this](MenuItem*)
    {
        MetaNode undoMetaBefore = GetComponent()->GetMeta();
        Component *copiedComp = EditorClipboard::GetCopiedComponent();
        copiedComp->CloneInto( GetComponent() );
        PushCurrentStateToUndoRedo(undoMetaBefore);
    });
    pasteValues->SetOverAndActionEnabled( (EditorClipboard::HasCopiedComponent()) );

    menuRootItem->AddSeparator();

    MenuItem *moveUp = menuRootItem->AddItem("Move Up");
    moveUp->SetSelectedCallback([this](MenuItem*)
    {
        MetaNode undoMetaBefore = GetComponent()->GetGameObject()->GetMeta();
        MoveComponent(GetComponent(), -1);
        PushCurrentStateToUndoRedoIfAnyChangeForGameObject(undoMetaBefore);
    });

    MenuItem *moveDown = menuRootItem->AddItem("Move Down");
    moveDown->SetSelectedCallback([this](MenuItem*)
    {
        MetaNode undoMetaBefore = GetComponent()->GetGameObject()->GetMeta();
        MoveComponent(GetComponent(), 1);
        PushCurrentStateToUndoRedoIfAnyChangeForGameObject(undoMetaBefore);
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

void ComponentInspectorWidget::OnComponentSet()
{
    // Empty
}

bool ComponentInspectorWidget::MustShowEnabledCheckbox() const
{
    return true;
}
