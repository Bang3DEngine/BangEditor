#ifndef UIINPUTCOLOR_H
#define UIINPUTCOLOR_H

#include "Bang/Vector4.h"
#include "Bang/GameObject.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IValueChangedListener.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputColor : public GameObject,
                     public EventEmitter<IValueChangedListener>
{
    GAMEOBJECT_EDITOR(UIInputColor);

protected:
	UIInputColor();
	virtual ~UIInputColor();

private:
    UILabel *p_label = nullptr;
    UIImageRenderer *p_colorImage = nullptr;
    UIButton *p_searchColorButton = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTCOLOR_H

