#ifndef AESNODE_H
#define AESNODE_H

#include "Bang/Bang.h"
#include "Bang/DPtr.h"
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

    void SetNodeName(const String &nodeName);
    void OnZoomScaleChanged(float zoomScale);

    const String& GetNodeName() const;
    UIFocusable* GetFocusable() const;
    uint GetIndexInStateMachine() const;
    AnimatorStateMachine *GetAnimatorSM() const;
    AnimatorStateMachineNode *GetSMNode() const;
    const Array<DPtr<AESConnectionLine>>& GetConnectionLines() const;

private:
    String m_nodeName = "";
    UIImageRenderer *p_bg = nullptr;
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_border = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    UITextRenderer *p_nodeNameText = nullptr;

    Vector2 m_grabOffset = Vector2::Zero;
    AnimatorSMEditorScene *p_aesScene = nullptr;

    Array<DPtr<AESNode>> p_toConnectedNodes;
    Array<DPtr<AESConnectionLine>> p_toConnectionLines;
    Map<AESConnectionLine*, AESNode*> p_toConnectionLineToConnectedNode;

    int m_framesPassedSinceLineDragStarted = 0;
    AESConnectionLine *p_toConnectionLineBeingDragged = nullptr;

    void CreateAndAddConnectionToBeginDrag();
    AESConnectionLine* CreateAndAddDefinitiveConnection();
    void OnDragConnectionLineEnd();
    void RemoveSelf();
    void Duplicate();
    void DestroyLineUsedForDragging();

    Vector2 GetExportPosition() const;
    void ImportPosition(const Vector2 &position);

    // IEventsAnimatorStateMachineNode
    virtual void OnConnectionAdded(const AnimatorStateMachineNode *node,
                                   const AnimatorStateMachineConnection *connection)
                 override;
    virtual void OnConnectionRemoved(const AnimatorStateMachineNode *node,
                                     const AnimatorStateMachineConnection *connection)
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

