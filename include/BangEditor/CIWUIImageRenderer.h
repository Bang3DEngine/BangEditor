#ifndef CIWUIIMAGERENDERER_H
#define CIWUIIMAGERENDERER_H

#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class IEventsValueChanged;
class UIImageRenderer;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputColor;
class UIInputFile;

class CIWUIImageRenderer : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(CIWUIImageRenderer);

public:
    CIWUIImageRenderer();

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    UIImageRenderer *GetUIImageRenderer() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

private:
    UIInputColor *p_tintInput = nullptr;
    UIInputFile *p_imageInput = nullptr;

    virtual ~CIWUIImageRenderer() override;
};
}

#endif  // CIWUIIMAGERENDERER_H
