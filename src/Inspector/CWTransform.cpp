#include "BangEditor/CWTransform.h"

#include "Bang/UILabel.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

#include "BangEditor/UIInputVector.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CWTransform::CWTransform(Transform *transform)
{
    SetName("CWTransform");
    SetTitle("Transform");

    GameObject *vlGo = GameObjectFactory::CreateUIGameObject("VLGo");
    UIVerticalLayout *vl = vlGo->AddComponent<UIVerticalLayout>();
    vl->SetSpacing(2);

    UIInputVector *posIV   = new UIInputVector("Position", 3);
    UIInputVector *rotIV   = new UIInputVector("Rotation", 3);
    UIInputVector *scaleIV = new UIInputVector("Scale   ", 3);

    posIV->Set(transform->GetLocalPosition());
    rotIV->Set(transform->GetLocalRotation().GetEulerAngles());
    scaleIV->Set(transform->GetLocalScale());

    GetContainer()->AddChild(vlGo);
    vlGo->AddChild(posIV);
    vlGo->AddChild(rotIV);
    vlGo->AddChild(scaleIV);
}

CWTransform::~CWTransform()
{
}

