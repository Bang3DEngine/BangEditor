#ifndef AESCONNECTIONLINE_H
#define AESCONNECTIONLINE_H

#include <array>
#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/DPtr.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/LineRenderer.h"
#include "Bang/String.h"
#include "BangEditor/AESNode.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class LineRenderer;
class AnimatorStateMachine;
class AnimatorStateMachineLayer;
class AnimatorStateMachineTransition;
class AnimatorStateMachineNode;
class UIImageRenderer;
}  // namespace Bang

using namespace Bang;
namespace BangEditor
{
class AESNode;
class UIContextMenu;

class AESConnectionLine : public GameObject
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(AESConnectionLine);

public:
    AESConnectionLine();
    virtual ~AESConnectionLine() override;

    // GameObject
    void BeforeRender() override;

    void SetNodeTo(AESNode *nodeTo);
    void SetNodeFrom(AESNode *nodeFrom);

    bool HasFocus() const;
    bool IsMouseOver() const;

    AESNode *GetAESNodeTo() const;
    AESNode *GetAESNodeFrom() const;
    AnimatorStateMachine *GetAnimatorSM() const;
    Array<AnimatorStateMachineTransition *> GetSMTransitions() const;

private:
    bool m_hasFocus = false;
    UIContextMenu *p_contextMenu = nullptr;
    LineRenderer *p_lineRenderer = nullptr;
    std::array<UIImageRenderer *, 3> p_arrowImgs;

    DPtr<AESNode> p_aesNodeTo;
    DPtr<AESNode> p_aesNodeFrom;

    void RemoveSelf();

    AESNode *GetFirstFoundNode() const;
    bool IsValidConnection(AESNode *oneNode, AESNode *otherNode) const;
    Vector3 GetConnectionPointLinePosition(AESNode *connPoint) const;
    void OffsetLinePositions() const;
};
}  // namespace BangEditor

#endif  // AESCONNECTIONLINE_H
