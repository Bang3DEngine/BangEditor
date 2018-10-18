#include "BangEditor/CIWBehaviour.h"

#include <map>
#include <utility>
#include <vector>

#include "Bang/Assert.h"
#include "Bang/BangPreprocessor.h"
#include "Bang/Behaviour.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/MetaAttribute.h"
#include "Bang/ReflectStruct.h"
#include "Bang/Serializable.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIInputText.h"
#include "Bang/UITextRenderer.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputVector.h"

namespace Bang
{
class IEventsValueChanged;
template <class>
class EventListener;
}

using namespace Bang;
using namespace BangEditor;

CIWBehaviour::CIWBehaviour()
{
}

CIWBehaviour::~CIWBehaviour()
{
}

void CIWBehaviour::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWBehaviour");
    SetTitle("Behaviour");

    SetLabelsWidth(80);
}

void CIWBehaviour::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    SetTitle(GetBehaviour()->GetClassName());
}

Behaviour *CIWBehaviour::GetBehaviour() const
{
    return SCAST<Behaviour *>(GetComponent());
}
