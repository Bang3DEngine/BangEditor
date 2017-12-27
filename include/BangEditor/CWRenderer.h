#ifndef CWRENDERER_H
#define CWRENDERER_H

#include "Bang/IValueChangedListener.h"

#include "BangEditor/ComponentWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Renderer;
FORWARD class UICheckBox;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class CWRenderer : public ComponentWidget,
                   public IValueChangedListener
{
public:
    void Update() override;

protected:
    CWRenderer();
    virtual ~CWRenderer();

    Renderer *GetRenderer() const;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;

    // ComponentWidget
    void SetComponent(Component *comp) override;

private:
    Renderer *p_relatedRenderer = nullptr;
    UICheckBox *p_visibleCheckBox = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // CWRENDERER_H

