#ifndef CIWRENDERER_H
#define CIWRENDERER_H

#include "Bang/IValueChangedListener.h"

#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Renderer;
FORWARD class UICheckBox;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;

class CIWRenderer : public ComponentInspectorWidget
{
public:
    // InspectorWidget
    virtual void Init() override;

    // ComponentInspectorWidget
    virtual void UpdateValuesFromComponent() override;

protected:
    CIWRenderer();
    virtual ~CIWRenderer();

    Renderer *GetRenderer() const;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;

private:
    UICheckBox *p_visibleCheckBox = nullptr;
    UIInputFile *p_materialInputFile = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWRENDERER_H

