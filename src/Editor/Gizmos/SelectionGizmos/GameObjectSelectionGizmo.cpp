#include "BangEditor/GameObjectSelectionGizmo.h"

#include "Bang/Color.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/GL.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/HideFlags.h"
#include "Bang/RenderFactory.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/NotSelectableInEditor.h"
#include "BangEditor/SelectionFramebuffer.h"

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

            RenderFactory::Parameters params;
            params.thickness = 2.0f;
            params.color = Color::Orange;
            params.receivesLighting = false;
            RenderFactory::RenderOutline( GetReferencedGameObject(), params );

            gb->PopDepthStencilTexture();
        }
    }
}

