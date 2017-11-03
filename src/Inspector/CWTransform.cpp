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

    p_posIV   = new UIInputVector("Position", 3);
    p_rotIV   = new UIInputVector("Rotation", 3);
    p_scaleIV = new UIInputVector("Scale   ", 3);

    p_posIV->Set(transform->GetLocalPosition());
    p_rotIV->Set(transform->GetLocalRotation().GetEulerAngles());
    p_scaleIV->Set(transform->GetLocalScale());

    p_posIV->RegisterListener(this);
    p_rotIV->RegisterListener(this);
    p_scaleIV->RegisterListener(this);

    GetContainer()->AddChild(vlGo);
    vlGo->AddChild(p_posIV);
    vlGo->AddChild(p_rotIV);
    vlGo->AddChild(p_scaleIV);

    p_relatedTransform = transform;
}

CWTransform::~CWTransform()
{
}

void CWTransform::OnValueChanged(const IEventEmitter *emitter)
{
    p_relatedTransform->SetLocalPosition(p_posIV->GetVector3());
    p_relatedTransform->SetLocalEuler(p_rotIV->GetVector3());
    p_relatedTransform->SetLocalScale(p_scaleIV->GetVector3());

}

