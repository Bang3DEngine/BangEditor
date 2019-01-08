#ifndef UISCENETOOLBARDOWN_H
#define UISCENETOOLBARDOWN_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
class UIButton;
class UICheckBox;
class UIComboBox;
class UISlider;
class UITextRenderer;
class UIToolButton;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UISceneToolbarDown : public GameObject,
                           public EventListener<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(UISceneToolbarDown);

public:
    UISceneToolbarDown();
    virtual ~UISceneToolbarDown() override;

private:
    UISlider *p_zoomSpeedSlider = nullptr;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;
};
}

#endif  // UISCENETOOLBARDOWN_H
