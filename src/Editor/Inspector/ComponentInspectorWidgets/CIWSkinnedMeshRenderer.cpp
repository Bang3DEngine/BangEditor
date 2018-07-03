#include "BangEditor/CIWSkinnedMeshRenderer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWSkinnedMeshRenderer::CIWSkinnedMeshRenderer()
{
}

CIWSkinnedMeshRenderer::~CIWSkinnedMeshRenderer()
{
}

void CIWSkinnedMeshRenderer::InitInnerWidgets()
{
    CIWMeshRenderer::InitInnerWidgets();

    SetName("SkinnedMeshRenderer");
    SetTitle("Skinned Mesh Renderer");
}

