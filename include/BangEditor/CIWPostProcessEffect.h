#ifndef CIWPOSTPROCESSEFFECT_H
#define CIWPOSTPROCESSEFFECT_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class IEventsValueChanged;
class PostProcessEffect;
class UIComboBox;
class UIInputNumber;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFile;

class CIWPostProcessEffect : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWPostProcessEffect);

public:
    CIWPostProcessEffect() = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_priorityInput = nullptr;
    UIComboBox *p_typeComboBox = nullptr;
    UIInputFile *p_fragmentShaderInput = nullptr;

    PostProcessEffect *GetPostProcessEffect() const;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

    virtual ~CIWPostProcessEffect() override = default;
};
}

#endif  // CIWPOSTPROCESSEFFECT_H
