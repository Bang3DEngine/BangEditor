#include "BangEditor/AnimatorEditor.h"

#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIDirLayoutMovableSeparator.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

AnimatorEditor::AnimatorEditor()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *mainLE = AddComponent<UILayoutElement>();
    mainLE->SetFlexibleSize(Vector2::One);

    UIHorizontalLayout *mainHL = AddComponent<UIHorizontalLayout>();
    mainHL->SetChildrenVerticalStretch(Stretch::FULL);

    GameObject *nodesSceneContainer = GameObjectFactory::CreateUIGameObject();
    {
        UILayoutElement *nodesSceneLE = nodesSceneContainer->
                                        AddComponent<UILayoutElement>();
        nodesSceneLE->SetMinSize( Vector2i(200) );
        nodesSceneLE->SetFlexibleSize( Vector2::One );

        UIImageRenderer *nodesSceneBG = nodesSceneContainer->
                                        AddComponent<UIImageRenderer>();
        nodesSceneBG->SetTint(Color::DarkGray);
    }

    GameObject *inspectorContainer = GameObjectFactory::CreateUIGameObject();
    {
        UILayoutElement *inspectorLE = inspectorContainer->
                                       AddComponent<UILayoutElement>();
        inspectorLE->SetMinSize( Vector2i(200) );

        UIImageRenderer *inspectorBG = inspectorContainer->
                                       AddComponent<UIImageRenderer>();
        inspectorBG->SetTint(Color::LightGray);
    }

    nodesSceneContainer->SetParent(this);
    GameObjectFactory::CreateUIDirLayoutMovableHSeparator()->
                       GetGameObject()->SetParent(this);
    inspectorContainer->SetParent(this);
}

AnimatorEditor::~AnimatorEditor()
{
}

void AnimatorEditor::Update()
{
    GameObject::Update();
}

