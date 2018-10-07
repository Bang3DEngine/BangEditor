#ifndef ANIMATORSMEDITOR_H
#define ANIMATORSMEDITOR_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Animator;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AnimatorSMEditorScene;

class AnimatorSMEditor : public GameObject,
                         public EventListener<IEventsDestroy>
{
    GAMEOBJECT_EDITOR(AnimatorSMEditor);

public:
	AnimatorSMEditor();
	virtual ~AnimatorSMEditor();

    // GameObject
    void Update() override;

    void SetAnimator(Animator *animator);
    Animator* GetAnimator() const;

private:
    Animator *p_animator = nullptr;
    AnimatorSMEditorScene *p_animatorEditorScene = nullptr;

    void Clear();

    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // ANIMATORSMEDITOR_H

