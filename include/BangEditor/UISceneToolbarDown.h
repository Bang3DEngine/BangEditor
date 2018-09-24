#ifndef UISCENETOOLBARDOWN_H
#define UISCENETOOLBARDOWN_H

#include "Bang/GameObject.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD class UISlider;
FORWARD class Texture2D;
FORWARD class UICheckBox;
FORWARD class UIComboBox;
FORWARD class UIToolButton;
FORWARD class UITextRenderer;
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

