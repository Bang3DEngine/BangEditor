#ifndef AESNODE_H
#define AESNODE_H

#include <sys/types.h>
#include <functional>
#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsAnimatorStateMachineNode.h"
#include "Bang/IEventsFocus.h"
#include "Bang/Map.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Animator;
FORWARD class AnimatorStateMachine;
FORWARD class AnimatorStateMachineConnection;
FORWARD class AnimatorStateMachineNode;
FORWARD class IEventsAnimatorStateMachineNode;
FORWARD class UIFocusable;
FORWARD class UIImageRenderer;
FORWARD class UITextRenderer;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class AESConnectionLine;
FORWARD class AnimatorSMEditorScene;
FORWARD class UIContextMenu;

class AESNode : public GameObject,
                public EventListener<IEventsFocus>,
                public EventListener<IEventsAnimatorStateMachineNode>
{
    GAMEOBJECT_EDITOR(AESNode);

public:
	AESNode();
    virtual ~AESNode() override;

    // GameObject
    void Update() override;

    void SetAsEntryNode();
    void SetNodeName(const String &nodeName);
    void OnZoomScaleChanged(float zoomScale);

    const String& GetNodeName() const;
    UIFocusable* GetFocusable() const;
    uint GetIndexInStateMachine() const;
    Animator *GetCurrentAnimator() const;
    AnimatorSMEditorScene *GetAESScene() const;
    AnimatorStateMachine *GetAnimatorSM() const;
    AnimatorStateMachineNode *GetSMNode() const;
    const Array<AESConnectionLine*>& GetConnectionLines() const;

private:
    String m_nodeName = "";
    UIImageRenderer *p_bg = nullptr;
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_border = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    UITextRenderer *p_nodeNameText = nullptr;
    UITextRenderer *p_entryNodeText = nullptr;

    Vector2 m_grabOffset = Vector2::Zero;
    AnimatorSMEditorScene *p_aesScene = nullptr;
    Array<AESConnectionLine*> p_connectionLinesTo;
    Map<AnimatorStateMachineNode*, AESConnectionLine*>
        p_nodeConnectedToToConnectionLine;

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
    virtual void OnConnectionAdded(AnimatorStateMachineNode *node,
                                   AnimatorStateMachineConnection *connection)
                 override;
    virtual void OnConnectionRemoved(AnimatorStateMachineNode *node,
                                     AnimatorStateMachineConnection *connection)
                 override;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

    friend class AnimatorSMEditorScene;
};

NAMESPACE_BANG_EDITOR_END

#endif // AESNODE_H

