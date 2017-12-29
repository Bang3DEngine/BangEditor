#include "BangEditor/CIWTransform.h"

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

CIWTransform::CIWTransform()
{
    SetName("CWTransform");
    SetTitle("Transform");

    p_posIV   = GameObject::Create<UIInputVector>(3);
    p_rotIV   = GameObject::Create<UIInputVector>(3);
    p_scaleIV = GameObject::Create<UIInputVector>(3);

    p_posIV->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_rotIV->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_scaleIV->EventEmitter<IValueChangedListener>::RegisterListener(this);

    AddWidget("Position", p_posIV);
    AddWidget("Rotation", p_rotIV);
    AddWidget("Scale",    p_scaleIV);

    SetLabelsWidth(60);
}

CIWTransform::~CIWTransform()
{
}

void CIWTransform::Update()
{
    ComponentInspectorWidget::Update();

    IValueChangedListener::SetReceiveEvents(false);

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

    IValueChangedListener::SetReceiveEvents(true);
}

void CIWTransform::OnValueChanged(Object *object)
{
    p_relatedTransform->SetLocalPosition(p_posIV->GetVector3());
    p_relatedTransform->SetLocalEuler(p_rotIV->GetVector3());
    p_relatedTransform->SetLocalScale(p_scaleIV->GetVector3());
}

void CIWTransform::SetComponent(Component *comp)
{
    Transform *transform = DCAST<Transform*>(comp);
    ASSERT(transform);

    p_relatedTransform = transform;

    p_posIV->Set(transform->GetLocalPosition());
    p_rotIV->Set(transform->GetLocalRotation().GetEulerAngles());
    p_scaleIV->Set(transform->GetLocalScale());
}

