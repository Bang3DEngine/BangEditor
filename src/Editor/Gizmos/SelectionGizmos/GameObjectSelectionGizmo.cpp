#include "BangEditor/GameObjectSelectionGizmo.h"

#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/RenderFactory.h"

#include "BangEditor/EditorCamera.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/SelectionFramebuffer.h"
#include "BangEditor/NotSelectableInEditor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

GameObjectSelectionGizmo::GameObjectSelectionGizmo()
{
    SetName("GameObjectSelectionGizmo");

    AddComponent<HideInHierarchy>();
    AddComponent<NotSelectableInEditor>();
    GetHideFlags().SetOn(HideFlag::DONT_SERIALIZE);
    GetHideFlags().SetOn(HideFlag::DONT_CLONE);
}

GameObjectSelectionGizmo::~GameObjectSelectionGizmo()
{
}

void GameObjectSelectionGizmo::Render(RenderPass rp, bool renderChildren)
{
    GameObject::Render(rp, renderChildren);

    if (!GetReferencedGameObject()) { return; }

    SelectionFramebuffer *sfb = EditorCamera::GetInstance()->GetSelectionFramebuffer();
    if (!sfb || !GL::IsBound(sfb))
    {
        if (rp == RenderPass::OVERLAY)
        {
            GBuffer *gb = GEngine::GetActiveGBuffer();
            gb->PushDepthStencilTexture();
            gb->SetOverlayDepthStencil();

            RenderFactory::Reset();
            RenderFactory::SetThickness(2.0f);
            RenderFactory::SetColor(Color::Orange);
            RenderFactory::SetReceivesLighting(false);
            RenderFactory::RenderOutline( GetReferencedGameObject() );

            gb->PopDepthStencilTexture();
        }
    }
}

