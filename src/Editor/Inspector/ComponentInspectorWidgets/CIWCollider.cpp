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

    SetLabelsWidth(95);
}

void CIWCollider::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    Collider *collider = GetCollider();
}

void CIWCollider::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    Collider *collider = GetCollider();
}

Collider *CIWCollider::GetCollider() const
{
    return SCAST<Collider *>(GetComponent());
}
