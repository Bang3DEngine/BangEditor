#ifndef CIWTEXTRENDERER_H
#define CIWTEXTRENDERER_H

#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class IEventsValueChanged;
class UIComboBox;
class UIInputNumber;
class UIInputText;
class UITextRenderer;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputColor;
class UIInputFileWithPreview;

class CIWUITextRenderer : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWUITextRenderer);

public:
    CIWUITextRenderer() = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    UITextRenderer *GetUITextRenderer() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

private:
    UIInputText *p_contentInput = nullptr;
    UIInputNumber *p_sizeInput = nullptr;
    UIInputColor *p_colorInput = nullptr;
    UIComboBox *p_horizontalAlignmentInput = nullptr;
    UIComboBox *p_verticalAlignmentInput = nullptr;
    UIInputFileWithPreview *p_fontFileInput = nullptr;

    virtual ~CIWUITextRenderer() = default;
};
}

#endif  // CIWTEXTRENDERER_H
