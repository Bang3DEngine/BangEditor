#ifndef ANIMATOREDITORSCENE_H
#define ANIMATOREDITORSCENE_H

#include "Bang/Bang.h"
#include "Bang/DPtr.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsFocus.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AESNode;

class AnimatorEditorScene : public GameObject,
                            public EventListener<IEventsFocus>
{
    GAMEOBJECT_EDITOR(AnimatorEditorScene);

public:
	AnimatorEditorScene();
	virtual ~AnimatorEditorScene();

    // GameObject
    void Update() override;

private:
    UIFocusable *p_focusable = nullptr;
    DPtr<UIImageRenderer> p_border = nullptr;
    GameObject *p_mainContainer = nullptr;

    float m_zoomScale = 0.85f;
    Vector2i m_panning = Vector2i::Zero;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

};

NAMESPACE_BANG_EDITOR_END

#endif // ANIMATOREDITORSCENE_H

