#ifndef CIWTEXTRENDERER_H
#define CIWTEXTRENDERER_H

#include "Bang/IEventsValueChanged.h"

#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIInputText;
FORWARD class UIInputNumber;
FORWARD class UITextRenderer;
FORWARD class UIComboBox;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;
FORWARD class UIInputColor;

class CIWUITextRenderer : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWUITextRenderer);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    UITextRenderer *GetUITextRenderer() const;

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;

private:
    UIInputText *p_contentInput = nullptr;
    UIInputNumber *p_sizeInput = nullptr;
    UIInputColor *p_colorInput = nullptr;
    UIComboBox *p_horizontalAlignmentInput = nullptr;
    UIComboBox *p_verticalAlignmentInput = nullptr;
    UIInputFile *p_fontFileInput = nullptr;

    CIWUITextRenderer() = default;
    virtual ~CIWUITextRenderer() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWTEXTRENDERER_H

