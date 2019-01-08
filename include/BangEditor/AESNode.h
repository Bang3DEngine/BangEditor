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

namespace Bang
{
class Animator;
class AnimatorStateMachine;
class AnimatorStateMachineLayer;
class AnimatorStateMachineTransition;
class AnimatorStateMachineNode;
class IEventsAnimatorStateMachineNode;
class UIFocusable;
class UIImageRenderer;
class UITextRenderer;
}

using namespace Bang;
namespace BangEditor
{
class AESConnectionLine;
class AnimatorSMEditorScene;
class UIContextMenu;

class AESNode : public GameObject,
                public EventListener<IEventsFocus>,
                public EventListener<IEventsDestroy>,
                public EventListener<IEventsAnimatorStateMachineNode>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(AESNode);

public:
    AESNode();
    virtual ~AESNode() override;

    // GameObject
    void Update() override;

    void SetAsEntryNode();
    void SetNodeName(const String &nodeName);
    void OnZoomScaleChanged(float zoomScale);

    const String &GetNodeName() const;
    UIFocusable *GetFocusable() const;
    uint GetIndexInStateMachine() const;
    Animator *GetCurrentAnimator() const;
    AnimatorSMEditorScene *GetAESScene() const;
    AnimatorStateMachine *GetAnimatorSM() const;
    AnimatorStateMachineLayer *GetAnimatorSMLayer() const;
    AnimatorStateMachineNode *GetSMNode() const;
    const Array<AESConnectionLine *> &GetConnectionLines() const;

private:
    String m_nodeName = "";
    UIImageRenderer *p_bg = nullptr;
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_border = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    UITextRenderer *p_nodeNameText = nullptr;
    UITextRenderer *p_entryNodeText = nullptr;

    Vector2 m_grabOffset = Vector2::Zero();
    AnimatorSMEditorScene *p_aesScene = nullptr;
    AnimatorStateMachineNode *p_smNode = nullptr;
    Array<AESConnectionLine *> p_connectionLinesTo;
    Map<AnimatorStateMachineNode *, AESConnectionLine *>
        p_nodeConnectedToToConnectionLine;

    int m_framesPassedSinceLineDragStarted = 0;
    AESConnectionLine *p_toConnectionLineBeingDragged = nullptr;

    void CreateAndAddConnectionToBeginDrag();
    AESConnectionLine *CreateAndAddDefinitiveConnection();
    void OnDragConnectionLineEnd();
    void RemoveSelf();
    void Duplicate();
    void DestroyLineUsedForDragging();
    void SetSMNode(AnimatorStateMachineNode *smNode);

    Vector2 GetExportPosition() const;
    void ImportPosition(const Vector2 &position);

    // IEventsAnimatorStateMachineNode
    virtual void OnTransitionAdded(
        AnimatorStateMachineNode *node,
        AnimatorStateMachineTransition *transition) override;
    virtual void OnTransitionRemoved(
        AnimatorStateMachineNode *node,
        AnimatorStateMachineTransition *transition) override;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

    friend class AnimatorSMEditorScene;
};
}

#endif  // AESNODE_H
