#ifndef CIWRENDERER_H
#define CIWRENDERER_H

#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class IEventsValueChanged;
class Renderer;
class UICheckBox;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFileWithPreview;
class UIInputMaterial;

class CIWRenderer : public ComponentInspectorWidget
{
public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    CIWRenderer();
    virtual ~CIWRenderer() override;

    Renderer *GetRenderer() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

protected:
    UICheckBox *p_visibleCheckBox = nullptr;
    UICheckBox *p_castsShadowsCheckBox = nullptr;
    UICheckBox *p_receivesShadowsCheckBox = nullptr;
    UICheckBox *p_useReflectionProbesCheckBox = nullptr;
    UIInputFileWithPreview *p_materialInputFile = nullptr;
};
}

#endif  // CIWRENDERER_H
