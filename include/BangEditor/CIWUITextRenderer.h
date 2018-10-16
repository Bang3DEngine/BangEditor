#ifndef CIWTEXTRENDERER_H
#define CIWTEXTRENDERER_H

#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang {
class IEventsValueChanged;
template <class > class EventEmitter;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIComboBox;
FORWARD class UIInputNumber;
FORWARD class UIInputText;
FORWARD class UITextRenderer;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputColor;
FORWARD class UIInputFileWithPreview;

class CIWUITextRenderer : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWUITextRenderer);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    UITextRenderer *GetUITextRenderer() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

private:
    UIInputText *p_contentInput             = nullptr;
    UIInputNumber *p_sizeInput              = nullptr;
    UIInputColor *p_colorInput              = nullptr;
    UIComboBox *p_horizontalAlignmentInput  = nullptr;
    UIComboBox *p_verticalAlignmentInput    = nullptr;
    UIInputFileWithPreview *p_fontFileInput = nullptr;

    CIWUITextRenderer() = default;
    virtual ~CIWUITextRenderer() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWTEXTRENDERER_H

