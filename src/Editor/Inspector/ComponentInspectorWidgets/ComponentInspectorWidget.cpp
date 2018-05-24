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
}

void ComponentInspectorWidget::UpdateFromReference()
{
    p_enabledCheckBox->SetChecked( GetComponent()->IsEnabled() );
}

void ComponentInspectorWidget::SetComponent(Component *comp)
{
    ASSERT(!GetComponent());

    p_component = comp;
    SetTitle(GetComponent()->GetClassName());
    p_icon->SetImageTexture( GetComponentIconTexture().Get() );
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
    p_titleText->SetContent(title);
}

GameObject *ComponentInspectorWidget::CreateTitleGameObject()
{
    GameObject *titleHLGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *titleHL = titleHLGo->AddComponent<UIHorizontalLayout>();
    titleHL->SetSpacing(5);

    p_icon = GameObjectFactory::CreateUIImage(Color::White);
    GameObject *iconGo = p_icon->GetGameObject();
    UILayoutElement *iconLE = iconGo->AddComponent<UILayoutElement>();
    iconLE->SetPreferredSize( Vector2i(16) );
    p_icon->SetTint(Color::DarkGray);

    GameObject *titleTextGo = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *titleText = titleTextGo->AddComponent<UITextRenderer>();
    titleText->SetHorizontalAlign(HorizontalAlignment::LEFT);
    titleText->SetContent("InspectorWidget");
    titleText->SetTextSize(12);
    p_titleText = titleText;

    UILayoutElement *titleLE = titleTextGo->AddComponent<UILayoutElement>();
    titleLE->SetFlexibleWidth(99.9f);

    GameObject *enabledTextGo = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *enabledText = enabledTextGo->AddComponent<UITextRenderer>();
    enabledText->SetContent("Enabled");
    enabledText->SetTextSize(12);
    enabledTextGo->SetEnabled( MustShowEnabledCheckbox() );

    p_enabledCheckBox = GameObjectFactory::CreateUICheckBox();
    GameObject *enabledCheckBoxGo = p_enabledCheckBox->GetGameObject();
    p_enabledCheckBox->EventEmitter<IValueChangedListener>::RegisterListener(this);
    enabledCheckBoxGo->SetEnabled( MustShowEnabledCheckbox() );

    iconGo->SetParent(titleHLGo);
    titleTextGo->SetParent(titleHLGo);
    enabledTextGo->SetParent(titleHLGo);
    enabledCheckBoxGo->SetParent(titleHLGo);

    return titleHLGo;
}

bool ComponentInspectorWidget::CanBeRemovedFromContextMenu() const
{
    return true;
}

void ComponentInspectorWidget::OnValueChanged(Object *object)
{
    if (object == p_enabledCheckBox)
    {
        GetComponent()->SetEnabled( p_enabledCheckBox->IsChecked() );
    }
}

RH<Texture2D> ComponentInspectorWidget::GetComponentIconTexture() const
{
    if (GetComponent())
    {
        String componentName = GetComponent()->GetClassName();
        return EditorTextureFactory::GetComponentIcon(componentName);
    }
    return EditorTextureFactory::GetCubeIcon();
}

void ComponentInspectorWidget::OnCreateContextMenu(MenuItem *menuRootItem)
{
    menuRootItem->SetFontSize(12);

    MenuItem *remove = menuRootItem->AddItem("Remove");
    remove->SetSelectedCallback([this](MenuItem*)
    {
        GameObject *go = GetComponent()->GetGameObject();
        go->RemoveComponent(GetComponent());
    });
    menuRootItem->AddSeparator();

    MenuItem *copy = menuRootItem->AddItem("Copy");
    copy->SetSelectedCallback([this](MenuItem*)
    { EditorClipboard::CopyComponent( GetComponent() ); });

    MenuItem *cut = menuRootItem->AddItem("Cut");
    cut->SetSelectedCallback([this](MenuItem*)
    {
        EditorClipboard::CopyComponent( GetComponent() );
        Component::Destroy( GetComponent() );
    });

    MenuItem *paste = menuRootItem->AddItem("Paste");
    paste->SetSelectedCallback([this](MenuItem*)
    {
        Component *copiedComp = EditorClipboard::GetCopiedComponent();
        Component *newComponent = copiedComp->Clone();
        GetInspectedGameObject()->AddComponent(newComponent);
    });
    paste->SetOverAndActionEnabled( (EditorClipboard::HasCopiedComponent()) );

    MenuItem *pasteValues = menuRootItem->AddItem("Paste values");
    pasteValues->SetSelectedCallback([this](MenuItem*)
    {
        Component *copiedComp = EditorClipboard::GetCopiedComponent();
        copiedComp->CloneInto( GetComponent() );
    });
    pasteValues->SetOverAndActionEnabled( (EditorClipboard::HasCopiedComponent()) );

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

