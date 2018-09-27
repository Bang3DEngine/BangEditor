#ifndef CIWRENDERER_H
#define CIWRENDERER_H

#include "Bang/IEventsValueChanged.h"

#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Renderer;
FORWARD class UICheckBox;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputMaterial;
FORWARD class UIInputFileWithPreview;

class CIWRenderer : public ComponentInspectorWidget
{
public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    CIWRenderer();
    virtual ~CIWRenderer();

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

NAMESPACE_BANG_EDITOR_END

#endif // CIWRENDERER_H

