#ifndef RIWANIMATORSTATEMACHINE_H
#define RIWANIMATORSTATEMACHINE_H

#include "Bang/Bang.h"
#include "Bang/AnimatorStateMachine.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/RIWResource.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD class AnimatorStateMachine;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class RIWAnimatorStateMachine : public RIWResource<AnimatorStateMachine>
{
    GAMEOBJECT_EDITOR(RIWAnimatorStateMachine);

public:
    // InspectorWidget
    void Init() override;

private:
    UIButton *p_openInEditorButton = nullptr;

    RIWAnimatorStateMachine();
    virtual ~RIWAnimatorStateMachine();

    // RIWResource
    void UpdateInputsFromResource() override;
    Texture2D *GetIconTexture() const override;
    void OnValueChangedRIWResource(
                        EventEmitter<IEventsValueChanged> *object) override;

    AnimatorStateMachine *GetAnimatorStateMachine() const;
};

NAMESPACE_BANG_EDITOR_END

#endif // RIWANIMATORSTATEMACHINE_H

