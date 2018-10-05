#ifndef AESTRANSITIONENTRY_H
#define AESTRANSITIONENTRY_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsFocus.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIFocusable;
FORWARD class LineRenderer;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class AESTransitionEntry : public GameObject,
                           public EventListener<IEventsFocus>
{
    GAMEOBJECT_EDITOR(AESTransitionEntry);

public:
	AESTransitionEntry();
	virtual ~AESTransitionEntry();

    // GameObject
    void Update() override;

private:
    UIImageRenderer *p_bg = nullptr;
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_border = nullptr;
    Vector2i m_nodeSize = Vector2i(200, 75);
    LineRenderer *p_transitionLineRenderer = nullptr;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // AESTRANSITIONENTRY_H

