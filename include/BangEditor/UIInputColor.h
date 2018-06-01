#ifndef UIINPUTCOLOR_H
#define UIINPUTCOLOR_H

#include "Bang/Vector4.h"
#include "Bang/GameObject.h"
#include "Bang/EventEmitter.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END


USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputVector;
FORWARD class ColorPickerReporter;

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
    virtual ~UIInputColor();

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

