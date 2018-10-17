#include "BangEditor/CIWCollider.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Collider.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/Path.h"
#include "Bang/PhysicsMaterial.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
#include "Bang/UICheckBox.h"
#include "BangEditor/UIInputFileWithPreview.h"
#include "BangEditor/UIInputVector.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

CIWCollider::CIWCollider()
{
}

CIWCollider::~CIWCollider()
{
}

void CIWCollider::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWCollider");
    SetTitle("Collider");

    p_centerInput = GameObject::Create<UIInputVector>();
    p_centerInput->SetSize(3);
    p_centerInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_isTriggerInput = GameObjectFactory::CreateUICheckBox();
    p_isTriggerInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_physicsMaterialInput = GameObject::Create<UIInputFileWithPreview>();
    p_physicsMaterialInput->SetExtensions(
        {Extensions::GetPhysicsMaterialExtension()});
    p_physicsMaterialInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    p_physicsMaterialInput->SetZoomable(false);

    AddWidget("Is Trigger", p_isTriggerInput->GetGameObject());
    AddWidget("Physics Material", p_physicsMaterialInput);
    AddWidget("Center", p_centerInput);

    SetLabelsWidth(95);
}

void CIWCollider::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    Collider *collider = GetCollider();
    if (!p_centerInput->HasFocus())
    {
        p_centerInput->Set(collider->GetCenter());
    }

    p_isTriggerInput->SetChecked(collider->GetIsTrigger());

    p_physicsMaterialInput->SetPath(
        collider->GetActivePhysicsMaterial()
            ? collider->GetActivePhysicsMaterial()->GetResourceFilepath()
            : Path::Empty);
}

void CIWCollider::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    Collider *collider = GetCollider();
    collider->SetIsTrigger(p_isTriggerInput->IsChecked());
    collider->SetCenter(p_centerInput->GetVector3());

    if (p_physicsMaterialInput->GetPath().IsFile())
    {
        RH<PhysicsMaterial> phMat =
            Resources::Load<PhysicsMaterial>(p_physicsMaterialInput->GetPath());
        collider->SetPhysicsMaterial(phMat.Get());
    }
    else if (p_physicsMaterialInput->GetPath().IsEmpty())
    {
        collider->SetPhysicsMaterial(nullptr);
    }
    /*
    else
    {
        collider->SetPhysicsMaterial(nullptr);
    }
    */
}

Collider *CIWCollider::GetCollider() const
{
    return SCAST<Collider *>(GetComponent());
}
