#ifndef AESNODE_H
#define AESNODE_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsFocus.h"
#include "Bang/IEventsAnimatorStateMachineNode.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIFocusable;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD class AnimatorStateMachine;
FORWARD class AnimatorStateMachineNode;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIContextMenu;
FORWARD class AESConnectionLine;
FORWARD class AnimatorSMEditorScene;

class AESNode : public GameObject,
                public EventListener<IEventsFocus>,
                public EventListener<IEventsDestroy>,
                public EventListener<IEventsAnimatorStateMachineNode>
{
    GAMEOBJECT_EDITOR(AESNode);

public:
	AESNode();
    virtual ~AESNode();

    // GameObject
    void Update() override;

    void OnZoomScaleChanged(float zoomScale);

    UIFocusable* GetFocusable() const;
    uint GetIndexInStateMachine() const;
    const Array<AESConnectionLine*>& GetConnectionLines() const;

private:
    UIImageRenderer *p_bg = nullptr;
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_border = nullptr;
    UITextRenderer *p_nameText = nullptr;
    UIContextMenu *p_contextMenu = nullptr;

    AnimatorSMEditorScene *p_animatorSMEditorScene = nullptr;

    Array<AESNode*> p_toConnectedNodes;
    Array<AESConnectionLine*> p_toConnectionLines;
    Map<AESConnectionLine*, AESNode*> p_toConnectionLineToConnectedNode;

    int m_framesPassedSinceLineDragStarted = 0;
    AESConnectionLine *p_toConnectionLineBeingDragged = nullptr;

    void CreateAndAddConnectionToBeginDrag();
    AESConnectionLine* CreateAndAddDefinitiveConnection();
    void OnDragConnectionLineEnd();
    void RemoveSelf();
    void Duplicate();
    void DestroyLineUsedForDragging();

    AnimatorStateMachine *GetAnimatorSM() const;
    AnimatorStateMachineNode *GetSMNode() const;


    // IEventsAnimatorStateMachineNode
    virtual void OnConnectionAdded(AnimatorStateMachineNode *node,
                                   AnimatorStateMachineConnection *connection)
                 override;
    virtual void OnConnectionRemoved(AnimatorStateMachineNode *node,
                                     AnimatorStateMachineConnection *connection)
                 override;

    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

    friend class AnimatorSMEditorScene;
};

NAMESPACE_BANG_EDITOR_END

#endif // AESNODE_H

