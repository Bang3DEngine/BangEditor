#ifndef CIWTRANSFORM_H
#define CIWTRANSFORM_H

#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class IEventsValueChanged;
class Transform;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputVector;

class CIWTransform : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWTransform);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    CIWTransform() = default;
    virtual ~CIWTransform() = default;

    virtual bool CanBeRemovedFromContextMenu() const override;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

private:
    UIInputVector *p_posIV = nullptr;
    UIInputVector *p_rotIV = nullptr;
    UIInputVector *p_scaleIV = nullptr;

    Transform *GetTransform() const;

    // ComponentWidget
    bool MustShowEnabledCheckbox() const override;
};
}

#endif  // CIWTRANSFORM_H
