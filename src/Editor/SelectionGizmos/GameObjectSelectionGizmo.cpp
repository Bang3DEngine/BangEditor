#include "BangEditor/GameObjectSelectionGizmo.h"

#include "Bang/Gizmos.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/SelectionFramebuffer.h"

#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/NotSelectableInEditor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

GameObjectSelectionGizmo::GameObjectSelectionGizmo()
{
    SetName("GameObjectSelectionGizmo");

    AddComponent<HideInHierarchy>();
    AddComponent<NotSelectableInEditor>();
    GetHideFlags().SetOn(HideFlag::DontSerialize);
    GetHideFlags().SetOn(HideFlag::DontClone);
}

GameObjectSelectionGizmo::~GameObjectSelectionGizmo()
{
}

void GameObjectSelectionGizmo::Render(RenderPass rp, bool renderChildren)
{
    GameObject::Render(rp, renderChildren);

    if (!GetReferencedGameObject()) { return; }

    SelectionFramebuffer *sfb = GEngine::GetActiveSelectionFramebuffer();
    if (!sfb || !GL::IsBound(sfb))
    {
        if (rp == RenderPass::Overlay)
        {
            GBuffer *gb = GEngine::GetActiveGBuffer();
            gb->PushDepthStencilTexture();
            gb->SetOverlayDepthStencil();

            Gizmos::Reset();
            Gizmos::SetThickness(2.0f);
            Gizmos::SetColor(Color::Orange);
            Gizmos::SetReceivesLighting(false);
            Gizmos::RenderOutline( GetReferencedGameObject() );

            gb->PopDepthStencilTexture();
        }
    }
}

