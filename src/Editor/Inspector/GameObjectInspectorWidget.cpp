#include "BangEditor/GameObjectInspectorWidget.h"

#include "Bang/MetaNode.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIInputText.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoSerializableChange.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

GameObjectInspectorWidget::GameObjectInspectorWidget()
{
}

GameObjectInspectorWidget::~GameObjectInspectorWidget()
{
}

void GameObjectInspectorWidget::SetGameObject(GameObject *gameObject)
{
    ASSERT(!GetGameObject());

    p_gameObject = gameObject;
    Update();
}

GameObject *GameObjectInspectorWidget::GetGameObject() const
{
    return p_gameObject;
}

void GameObjectInspectorWidget::InitInnerWidgets()
{
    InspectorWidget::InitInnerWidgets();

    SetName("GameObjectInspectorWidget");
    SetTitle("GameObject");

    GetInspectorWidgetTitle()->GetEnabledCheckBox()->
                               GetGameObject()->SetEnabled(false);
    GetInspectorWidgetTitle()->GetIcon()->GetGameObject()->SetEnabled(false);
    GetInspectorWidgetTitle()->SetEnabled(false);

    p_nameInput = GameObjectFactory::CreateUIInputText();
    p_nameInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_enabledInput = GameObjectFactory::CreateUICheckBox();
    p_enabledInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Name",    p_nameInput->GetGameObject());
    AddWidget("Enabled", p_enabledInput->GetGameObject());
    SetLabelsWidth(80);
}

void GameObjectInspectorWidget::UpdateFromReference()
{
    p_nameInput->GetText()->SetContent( GetGameObject()->GetName() );
    p_enabledInput->SetChecked( GetGameObject()->IsEnabled() );
}

void GameObjectInspectorWidget::OnValueChanged(EventEmitter<IEventsValueChanged> *object)
{
    MetaNode undoMetaBefore = GetGameObject()->GetMeta();

    if (object == p_nameInput)
    {
        GetGameObject()->SetName( p_nameInput->GetText()->GetContent() );
    }
    else if (object == p_enabledInput)
    {
        GetGameObject()->SetEnabled(p_enabledInput->IsChecked());
    }

    MetaNode currentMeta = GetGameObject()->GetMeta();
    UndoRedoManager::PushAction( new UndoRedoSerializableChange(GetGameObject(),
                                                                undoMetaBefore,
                                                                currentMeta) );
}

