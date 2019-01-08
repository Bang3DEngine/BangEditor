#ifndef AIWANIMATORSTATEMACHINE_H
#define AIWANIMATORSTATEMACHINE_H

#include <vector>

#include "Bang/AnimatorStateMachine.h"
#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/String.h"
#include "BangEditor/AIWAsset.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
class UIButton;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class AIWAnimatorStateMachine : public AIWAsset<AnimatorStateMachine>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(AIWAnimatorStateMachine);

public:
    AIWAnimatorStateMachine();

    // InspectorWidget
    void Init() override;

private:
    UIButton *p_openInEditorButton = nullptr;

    virtual ~AIWAnimatorStateMachine() override;

    // AIWAsset
    void UpdateInputsFromAsset() override;
    Texture2D *GetIconTexture() const override;
    void OnValueChangedAIWAsset(
        EventEmitter<IEventsValueChanged> *object) override;

    AnimatorStateMachine *GetAnimatorStateMachine() const;
};
}

#endif  // AIWANIMATORSTATEMACHINE_H
