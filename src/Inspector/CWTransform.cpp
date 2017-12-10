#include "BangEditor/CWTransform.h"

#include "Bang/UILabel.h"
#include "Bang/UICanvas.h"
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

    GameObject *vlGo = GameObjectFactory::CreateUIGameObjectNamed("VLGo");
    UIVerticalLayout *vl = vlGo->AddComponent<UIVerticalLayout>();
    vl->SetSpacing(2);

    p_posIV   = GameObject::Create<UIInputVector>("Position", 3);
    p_rotIV   = GameObject::Create<UIInputVector>("Rotation", 3);
    p_scaleIV = GameObject::Create<UIInputVector>("Scale   ", 3);

    p_posIV->Set(transform->GetLocalPosition());
    p_rotIV->Set(transform->GetLocalRotation().GetEulerAngles());
    p_scaleIV->Set(transform->GetLocalScale());

    UIInputVector* inputVectors[] = {p_posIV, p_rotIV, p_scaleIV};
    for (UIInputVector *inputVector : inputVectors)
    {
        inputVector->SetPreferredWidth(50);
        inputVector->EventEmitter<IValueChangedListener>::RegisterListener(this);
        vlGo->SetAsChild(inputVector);
    }

    GetContainer()->SetAsChild(vlGo);

    p_relatedTransform = transform;
}

CWTransform::~CWTransform()
{
}

void CWTransform::Update()
{
    ComponentWidget::Update();

    if (!p_posIV->HasFocus())
    {
        p_posIV->Set(p_relatedTransform->GetLocalPosition());
    }

    if (!p_rotIV->HasFocus())
    {
        p_rotIV->Set(p_relatedTransform->GetLocalEuler());
    }

    if (!p_scaleIV->HasFocus())
    {
        p_scaleIV->Set(p_relatedTransform->GetLocalScale());
    }
}

void CWTransform::OnValueChanged(Object *object)
{
    p_relatedTransform->SetLocalPosition(p_posIV->GetVector3());
    p_relatedTransform->SetLocalEuler(p_rotIV->GetVector3());
    p_relatedTransform->SetLocalScale(p_scaleIV->GetVector3());
}

