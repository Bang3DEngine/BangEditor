#ifndef CIWPOSTPROCESSEFFECT_H
#define CIWPOSTPROCESSEFFECT_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang {
class IEventsValueChanged;
template <class > class EventEmitter;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class PostProcessEffect;
FORWARD class UIComboBox;
FORWARD class UIInputNumber;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFileWithPreview;

class CIWPostProcessEffect : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWPostProcessEffect);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_priorityInput                = nullptr;
    UIComboBox *p_typeComboBox                    = nullptr;
    UIInputFileWithPreview *p_fragmentShaderInput = nullptr;

    PostProcessEffect* GetPostProcessEffect() const;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    CIWPostProcessEffect() = default;
    virtual ~CIWPostProcessEffect() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWPOSTPROCESSEFFECT_H

