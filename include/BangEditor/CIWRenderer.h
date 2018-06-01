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

FORWARD class UIInputFile;
FORWARD class UIInputMaterial;

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

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;

private:
    UICheckBox *p_visibleCheckBox = nullptr;
    UICheckBox *p_castsShadowsCheckBox = nullptr;
    UICheckBox *p_receivesShadowsCheckBox = nullptr;
    UIInputMaterial *p_materialInputFile = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWRENDERER_H

