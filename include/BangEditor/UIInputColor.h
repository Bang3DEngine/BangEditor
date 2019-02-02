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

namespace Bang
{
class IEventsValueChanged;
class UIButton;
class UIImageRenderer;
}

using namespace Bang;
namespace BangEditor
{
class ColorPickerReporter;
class UIInputVector;

class UIInputColor : public GameObject,
                     public EventListener<IEventsValueChanged>,
                     public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(UIInputColor);

public:
    UIInputColor();

    void SetColor(const Color &color);

    const Color &GetColor() const;
    bool HasFocus() const;
    UIButton *GetSearchColorButton() const;

protected:
    virtual ~UIInputColor() override;

    // GameObject
    void Update() override;

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;

private:
    Color m_color = Color::Zero();
    ColorPickerReporter *m_colorPickerReporter = nullptr;

    UIImageRenderer *p_colorImage = nullptr;
    UIImageRenderer *p_bgCheckerboardImage = nullptr;
    UIButton *p_searchColorButton = nullptr;
};
}

#endif  // UIINPUTCOLOR_H
