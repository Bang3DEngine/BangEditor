#ifndef CIWUIIMAGERENDERER_H
#define CIWUIIMAGERENDERER_H

#include "Bang/IEventsValueChanged.h"

#include "BangEditor/ComponentInspectorWidget.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputColor;
FORWARD class UIInputTexture;

class CIWUIImageRenderer : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWUIImageRenderer);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    UIImageRenderer *GetUIImageRenderer() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

private:
    UIInputColor *p_tintInput = nullptr;
    UIInputTexture *p_imageInput = nullptr;

	CIWUIImageRenderer();
	virtual ~CIWUIImageRenderer();
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWUIIMAGERENDERER_H

