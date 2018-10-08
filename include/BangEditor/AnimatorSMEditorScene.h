#ifndef ANIMATOREDITORSCENE_H
#define ANIMATOREDITORSCENE_H

#include "Bang/Bang.h"
#include "Bang/DPtr.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsFocus.h"
#include "Bang/ResourceHandle.h"
#include "Bang/AnimatorStateMachine.h"
#include "Bang/IEventsAnimatorStateMachine.h"
#include "Bang/IEventsAnimatorStateMachineNode.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AESNode;
FORWARD class UIContextMenu;

class AnimatorSMEditorScene : public GameObject,
                              public EventListener<IEventsFocus>,
                              public EventListener<IEventsAnimatorStateMachine>,
                              public EventListener<IEventsAnimatorStateMachineNode>
{
    GAMEOBJECT_EDITOR(AnimatorSMEditorScene);

public:
    AnimatorSMEditorScene();
    virtual ~AnimatorSMEditorScene();

    // GameObject
    void Update() override;

    void CreateAndAddNode(AnimatorStateMachineNode *smNode, uint addIdx);
    void SetAnimatorSM(AnimatorStateMachine *animatorSM);
    void Clear();

    const Array<AESNode*>& GetAESNodes() const;
    AnimatorStateMachine* GetAnimatorSM() const;

private:
    RH<AnimatorStateMachine> p_animatorSM;

    Array<AESNode*> p_nodes;
    UIFocusable *p_focusable = nullptr;
    GameObject *p_mainContainer = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    DPtr<UIImageRenderer> p_border = nullptr;

    float m_zoomScale = 0.85f;
    Vector2i m_panning = Vector2i::Zero;

    void PropagateOnZoomScaleChanged();
    bool IsMouseOverSomeConnectionLine() const;

    // IEventsAnimatorStateMachine
    virtual void OnNodeCreated(AnimatorStateMachine *stateMachine,
                               uint newNodeIdx,
                               AnimatorStateMachineNode *newNode) override;
    virtual void OnNodeRemoved(AnimatorStateMachine *stateMachine,
                               uint removedNodeIdx,
                               AnimatorStateMachineNode *removedNode) override;

    // IEventsAnimatorStateMachineNode
    virtual void OnConnectionAdded(AnimatorStateMachineNode *node,
                                   AnimatorStateMachineConnection *connection)
                 override;
    virtual void OnConnectionRemoved(AnimatorStateMachineNode *node,
                                     AnimatorStateMachineConnection *connection)
                 override;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

};

NAMESPACE_BANG_EDITOR_END

#endif // ANIMATOREDITORSCENE_H

