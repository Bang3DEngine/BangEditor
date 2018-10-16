#ifndef UIINPUTCOLOR_H
#define UIINPUTCOLOR_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "Bang/Vector4.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class IEventsValueChanged;
FORWARD class UIButton;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ColorPickerReporter;
FORWARD class UIInputVector;

class UIInputColor : public GameObject,
                     public EventListener<IEventsValueChanged>,
                     public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(UIInputColor);

public:
    void SetColor(const Color &color);

    const Color& GetColor() const;
    bool HasFocus() const;

protected:
	UIInputColor();
    virtual ~UIInputColor() override;

    // GameObject
    void Update() override;

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;

private:
    Color m_color = Color::Zero;
    ColorPickerReporter *m_colorPickerReporter = nullptr;

    UIImageRenderer *p_colorImage = nullptr;
    UIImageRenderer *p_bgCheckerboardImage = nullptr;
    UIButton *p_searchColorButton = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTCOLOR_H

