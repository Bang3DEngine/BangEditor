#ifndef AESNODE_H
#define AESNODE_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsFocus.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIFocusable;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AESTransitionEntry;

class AESNode : public GameObject,
                public EventListener<IEventsFocus>
{
    GAMEOBJECT_EDITOR(AESNode);

public:
	AESNode();
    virtual ~AESNode();

    // GameObject
    void Update() override;

private:
    AESTransitionEntry *p_inEntry  = nullptr;
    AESTransitionEntry *p_outEntry = nullptr;

    UIImageRenderer *p_bg = nullptr;
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_border = nullptr;
    Vector2i m_nodeSize = Vector2i(200, 75);

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // AESNODE_H

