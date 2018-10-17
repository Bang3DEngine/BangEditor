#include "BangEditor/CIWSkinnedMeshRenderer.h"

using namespace Bang;
using namespace BangEditor;

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
