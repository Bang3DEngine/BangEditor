#ifndef ANIMATORSMEDITOR_H
#define ANIMATORSMEDITOR_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/ResourceHandle.h"
#include "Bang/String.h"
#include "Bang/Time.h"
#include "Bang/UIButton.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class Animator;
FORWARD   class AnimatorStateMachine;
FORWARD_T class EventEmitter;
FORWARD   class IEventsValueChanged;
FORWARD   class UIButton;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AnimatorSMEditorScene;
FORWARD class UIInputArray;

class AnimatorSMEditor : public GameObject,
                         public EventListener<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(AnimatorSMEditor);

public:
	AnimatorSMEditor();
	virtual ~AnimatorSMEditor() override;

    // GameObject
    void Update() override;

    void SetAnimatorSM(AnimatorStateMachine *animatorSM);

    AnimatorStateMachine* GetAnimatorSM() const;

private:
    Time m_lastVariablesInputUpdateTime;
    RH<AnimatorStateMachine> p_animatorSM;
    AnimatorSMEditorScene *p_animatorEditorScene = nullptr;

    UIButton *p_centerSceneButton = nullptr;
    UIInputArray *p_variablesInput = nullptr;

    void Clear();

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // ANIMATORSMEDITOR_H

