#ifndef ANIMATOREDITORSCENE_H
#define ANIMATOREDITORSCENE_H

#include <sys/types.h>
#include <vector>

#include "Bang/AnimatorStateMachine.h"
#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/DPtr.h"
#include "Bang/DPtr.tcc"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsAnimatorStateMachine.h"
#include "Bang/IEventsAnimatorStateMachineNode.h"
#include "Bang/IEventsFocus.h"
#include "Bang/ResourceHandle.h"
#include "Bang/String.h"
#include "Bang/Time.h"
#include "Bang/UIImageRenderer.h"
#include "BangEditor/BangEditor.h"

namespace Bang {
class AnimatorStateMachine;
class AnimatorStateMachineNode;
class IEventsAnimatorStateMachine;
class UIFocusable;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AESNode;
FORWARD class UIContextMenu;

class AnimatorSMEditorScene : public GameObject,
                              public EventListener<IEventsFocus>,
                              public EventListener<IEventsAnimatorStateMachine>
{
    GAMEOBJECT_EDITOR(AnimatorSMEditorScene);

public:
    AnimatorSMEditorScene();
    virtual ~AnimatorSMEditorScene();

    // GameObject
    void Update() override;

    void CreateAndAddNode(AnimatorStateMachineNode *smNode, uint addIdx);
    void SetAnimatorSM(AnimatorStateMachine *animatorSM);
    Vector2 GetMousePositionInSceneSpace() const;
    Vector2 GetWorldPositionInSceneSpace(const Vector2 &pos) const;
    void CenterScene();
    void Clear();

    float GetZoomScale() const;
    const Array<AESNode*>& GetAESNodes() const;
    AnimatorStateMachine* GetAnimatorSM() const;

private:
    RH<AnimatorStateMachine> p_animatorSM;

    Array<AESNode*> p_nodes;
    UIFocusable *p_focusable = nullptr;
    GameObject *p_gridContainer = nullptr;
    UIImageRenderer *p_gridImg = nullptr;
    GameObject *p_mainContainer = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    DPtr<UIImageRenderer> p_border = nullptr;
    Time m_lastTimeAnimatorSMWasExported = Time(0);

    float m_zoomScale = 1.0f;
    Vector2 m_panning = Vector2::Zero;

    void SetZoomScale(float zoomScale);
    void UpdatePanningAndZoomOnTransforms();
    bool IsMouseOverSomeConnectionLine() const;

    void ImportCurrentAnimatorStateMachineExtraInformation();
    void ExportCurrentAnimatorStateMachineIfAny();

    // IEventsAnimatorStateMachine
    virtual void OnNodeCreated(AnimatorStateMachine *stateMachine,
                               uint newNodeIdx,
                               AnimatorStateMachineNode *newNode) override;
    virtual void OnNodeRemoved(AnimatorStateMachine *stateMachine,
                               uint removedNodeIdx,
                               AnimatorStateMachineNode *removedNode) override;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

    friend class AnimatorSMEditor;
};

NAMESPACE_BANG_EDITOR_END

#endif // ANIMATOREDITORSCENE_H

