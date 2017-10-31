#include "BangEditor/CWTransform.h"

#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIContentSizeFitter.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CWTransform::CWTransform(Transform *transform)
{
    GameObjectFactory::CreateUIGameObjectInto(this);
    SetName("CWTransform");

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();

    GameObject *test = GameObjectFactory::CreateUIGameObject("test");
    UIImageRenderer *img = test->AddComponent<UIImageRenderer>();

    GameObject *textGo =GameObjectFactory::CreateUIGameObject("text");
    UITextRenderer *text = textGo->AddComponent<UITextRenderer>();
    text->SetContent( String( transform->GetScale().z ) );

    AddChild(test);
    AddChild(textGo);
}

CWTransform::~CWTransform()
{
}

