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

class CIWRenderer : public ComponentInspectorWidget,
                    public IValueChangedListener
{
public:
    void Update() override;

protected:
    CIWRenderer();
    virtual ~CIWRenderer();

    Renderer *GetRenderer() const;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;

    // ComponentWidget
    void SetComponent(Component *comp) override;

private:
    Renderer *p_relatedRenderer = nullptr;

    UICheckBox *p_visibleCheckBox = nullptr;
    UIInputFile *p_materialInputFile = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWRENDERER_H

