#ifndef ANIMATORSMEDITOR_H
#define ANIMATORSMEDITOR_H

#include "Bang/Bang.h"
#include "Bang/UIButton.h"
#include "Bang/GameObject.h"
#include "Bang/ResourceHandle.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Animator;
FORWARD class AnimatorStateMachine;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AnimatorSMEditorScene;

class AnimatorSMEditor : public GameObject
{
    GAMEOBJECT_EDITOR(AnimatorSMEditor);

public:
	AnimatorSMEditor();
	virtual ~AnimatorSMEditor();

    // GameObject
    void Update() override;

    void SetAnimatorSM(AnimatorStateMachine *animatorSM);
    AnimatorStateMachine* GetAnimatorSM() const;

private:
    RH<AnimatorStateMachine> p_animatorSM;
    AnimatorSMEditorScene *p_animatorEditorScene = nullptr;

    UIButton *p_centerSceneButton = nullptr;

    void Clear();
};

NAMESPACE_BANG_EDITOR_END

#endif // ANIMATORSMEDITOR_H

