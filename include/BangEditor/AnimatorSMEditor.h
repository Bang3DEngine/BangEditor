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
#include "Bang/AssetHandle.h"
#include "Bang/String.h"
#include "Bang/Time.h"
#include "Bang/UIButton.h"
#include "BangEditor/ASMLayerInput.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/UIInputArray.h"

namespace Bang
{
class Animator;
class AnimatorStateMachine;
template <class>
class EventEmitter;
class IEventsValueChanged;
class UIButton;
}

using namespace Bang;
namespace BangEditor
{
class AnimatorSMEditorScene;
class UIInputArray;

class AnimatorSMEditor : public GameObject,
                         public EventListener<IEventsValueChanged>,
                         public EventListener<IEventsASMLayerInput>,
                         public EventListener<IEventsUIInputArray>
{
    GAMEOBJECT_EDITOR(AnimatorSMEditor);

public:
    AnimatorSMEditor();
    virtual ~AnimatorSMEditor() override;

    // GameObject
    void Update() override;

    void SetAnimatorSM(AnimatorStateMachine *animatorSM);

    AnimatorStateMachine *GetAnimatorSM() const;

private:
    Time m_lastVariablesInputUpdateTime;
    AH<AnimatorStateMachine> p_animatorSM;
    AnimatorSMEditorScene *p_animatorEditorScene = nullptr;

    GameObject *p_sidebar = nullptr;
    UIButton *p_centerSceneButton = nullptr;
    UIInputArray *p_layersInput = nullptr;
    UIInputArray *p_variablesInput = nullptr;

    void Clear();

    // IEventsUIInputArray
    void OnInputRowMoved(UIInputArray *inputArray,
                         Serializable *inputRow,
                         uint oldIndex,
                         uint newIndex) override;

    // IEventsASMLayerInput
    void OnLayerInputSelected(ASMLayerInput *selectedLayerInput) override;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;
};
}

#endif  // ANIMATORSMEDITOR_H
