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

void CIWTransform::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWTransform");
    SetTitle("Transform");

    p_posIV   = GameObject::Create<UIInputVector>(3);
    p_rotIV   = GameObject::Create<UIInputVector>(3);
    p_scaleIV = GameObject::Create<UIInputVector>(3);

    p_posIV->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_rotIV->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_scaleIV->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Position", p_posIV);
    AddWidget("Rotation", p_rotIV);
    AddWidget("Scale",    p_scaleIV);

    SetLabelsWidth(60);
}

void CIWTransform::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    if (!p_posIV->HasFocus())
    {
        p_posIV->Set(GetTransform()->GetLocalPosition());
    }

    if (!p_rotIV->HasFocus())
    {
        p_rotIV->Set(GetTransform()->GetLocalEuler());
    }

    if (!p_scaleIV->HasFocus())
    {
        p_scaleIV->Set(GetTransform()->GetLocalScale());
    }
}

bool CIWTransform::CanBeRemovedFromContextMenu() const
{
    return false;
}

Transform *CIWTransform::GetTransform() const
{
    return SCAST<Transform*>( GetComponent() );
}

void CIWTransform::OnValueChanged(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChanged(object);

    GetTransform()->SetLocalPosition(p_posIV->GetVector3());
    GetTransform()->SetLocalEuler(p_rotIV->GetVector3());
    GetTransform()->SetLocalScale(p_scaleIV->GetVector3());
}

bool CIWTransform::MustShowEnabledCheckbox() const
{
    return false;
}


