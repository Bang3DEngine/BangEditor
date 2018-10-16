#ifndef CIWUIIMAGERENDERER_H
#define CIWUIIMAGERENDERER_H

#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang {
class IEventsValueChanged;
class UIImageRenderer;
template <class > class EventEmitter;
}  // namespace Bang

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

