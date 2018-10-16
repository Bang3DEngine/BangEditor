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

namespace Bang {
class IEventsValueChanged;
template <class > class EventEmitter;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD class UIButton;
FORWARD class UICheckBox;
FORWARD class UIComboBox;
FORWARD class UISlider;
FORWARD class UITextRenderer;
FORWARD class UIToolButton;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UISceneToolbarDown : public GameObject,
                           public EventListener<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(UISceneToolbarDown);

public:
	UISceneToolbarDown();
	virtual ~UISceneToolbarDown();

private:
    UISlider *p_zoomSpeedSlider = nullptr;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENETOOLBARDOWN_H

