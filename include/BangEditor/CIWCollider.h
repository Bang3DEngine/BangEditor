#ifndef CIWCOLLIDER_H
#define CIWCOLLIDER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class Collider;
class UIInputNumber;
class IEventsValueChanged;
class UICheckBox;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFileWithPreview;
class UIInputVector;

class CIWCollider : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWCollider);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;

    // ComponentInspectorWidget
    virtual void UpdateFromReference() override;

protected:
    CIWCollider();
    virtual ~CIWCollider() override;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

    Collider *GetCollider() const;

private:
    UICheckBox *p_isTriggerInput = nullptr;
    UIInputVector *p_centerInput = nullptr;
    UIInputFileWithPreview *p_physicsMaterialInput = nullptr;

    friend class ComponentInspectorWidgetFactory;
};
}

#endif  // CIWCOLLIDER_H
