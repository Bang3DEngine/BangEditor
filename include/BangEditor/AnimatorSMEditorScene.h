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
#include "Bang/IEventsAnimatorStateMachineLayer.h"
#include "Bang/IEventsAnimatorStateMachineNode.h"
#include "Bang/IEventsFocus.h"
#include "Bang/ResourceHandle.h"
#include "Bang/String.h"
#include "Bang/Time.h"
#include "Bang/UIImageRenderer.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class AnimatorStateMachineLayer;
class AnimatorStateMachineNode;
class IEventsAnimatorStateMachine;
class UIFocusable;
class UIImageRenderer;
}

using namespace Bang;
namespace BangEditor
{
class AESNode;
class UIContextMenu;

class AnimatorSMEditorScene
    : public GameObject,
      public EventListener<IEventsFocus>,
      public EventListener<IEventsAnimatorStateMachineLayer>
{
    GAMEOBJECT_EDITOR(AnimatorSMEditorScene);

public:
    AnimatorSMEditorScene();
    virtual ~AnimatorSMEditorScene() override;

    // GameObject
    void Update() override;

    void CreateAndAddNode(AnimatorStateMachineNode *smNode, uint addIdx);
    void SetAnimatorSMLayer(AnimatorStateMachineLayer *animatorSMLayer);
    Vector2 GetMousePositionInSceneSpace() const;
    Vector2 GetWorldPositionInSceneSpace(const Vector2 &pos) const;
    void CenterScene();
    void Clear();

    float GetZoomScale() const;
    const Array<AESNode *> &GetAESNodes() const;
    AnimatorStateMachine *GetAnimatorSM() const;
    AnimatorStateMachineLayer *GetAnimatorSMLayer() const;

private:
    AnimatorStateMachineLayer *p_animatorSMLayer = nullptr;

    Array<AESNode *> p_nodes;
    UIFocusable *p_focusable = nullptr;
    GameObject *p_gridContainer = nullptr;
    UIImageRenderer *p_gridImg = nullptr;
    GameObject *p_zoomContainer = nullptr;
    GameObject *p_mainContainer = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    DPtr<UIImageRenderer> p_border = nullptr;
    Time m_lastTimeAnimatorSMWasExported = Time(0);

    uint m_framesAfterNewAnimatorSMSetAndVisible = 0;

    float m_zoomScale = 1.0f;
    Vector2 m_panning = Vector2::Zero;

    void SetZoomScale(float zoomScale, bool centerOnMouse);
    void UpdatePanningAndZoomOnTransforms();
    bool IsMouseOverSomeConnectionLine() const;

    void ImportCurrentAnimatorStateMachineExtraInformation();
    void ExportCurrentAnimatorStateMachineIfAny();

    // IEventsAnimatorStateMachineLayer
    virtual void OnNodeCreated(uint newNodeIdx,
                               AnimatorStateMachineNode *newNode) override;
    virtual void OnNodeRemoved(uint removedNodeIdx,
                               AnimatorStateMachineNode *removedNode) override;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

    friend class AnimatorSMEditor;
};
}

#endif  // ANIMATOREDITORSCENE_H
