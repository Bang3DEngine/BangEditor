#ifndef RECTTRANSFORMANCHORSELECTIONGIZMO_H
#define RECTTRANSFORMANCHORSELECTIONGIZMO_H

#include "Bang/Alignment.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/RenderPass.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

namespace Bang
{
class GameObject;
class UIImageRenderer;
}

using namespace Bang;
namespace BangEditor
{
class RectTransformAnchorSelectionGizmo : public SelectionGizmo
{
    GAMEOBJECT_EDITOR(RectTransformAnchorSelectionGizmo);

public:
    enum class AnchorSide
    {
        LEFT_BOT,
        LEFT_TOP,
        RIGHT_TOP,
        RIGHT_BOT
    };

    RectTransformAnchorSelectionGizmo();

    // GameObject
    void Update() override;
    void Render(RenderPass renderPass, bool renderChildren) override;

    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

    void SetAnchorSide(AnchorSide anchorSide);

private:
    static const int AnchorSize;
    static const int AnchorSelectionSize;

    AnchorSide m_anchorSide = AnchorSide::LEFT_BOT;
    Vector2 m_startGrabMousePos = Vector2::Zero();
    Vector2 m_startLocalAnchorMin = Vector2::Zero();
    Vector2 m_startLocalAnchorMax = Vector2::Zero();

    GameObject *p_anchorGO = nullptr;
    GameObject *p_selectionGO = nullptr;
    UIImageRenderer *p_anchorRenderer = nullptr;
    UIImageRenderer *p_selectionRenderer = nullptr;

    virtual ~RectTransformAnchorSelectionGizmo() override;

    void UpdateBasedOnAnchorSide();
};
}

#endif  // RECTTRANSFORMANCHORSELECTIONGIZMO_H
