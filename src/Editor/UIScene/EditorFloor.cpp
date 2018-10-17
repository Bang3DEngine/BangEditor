#include "BangEditor/EditorFloor.h"

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/Color.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/GameObject.tcc"
#include "Bang/LineRenderer.h"
#include "Bang/Material.h"
#include "Bang/MaterialFactory.h"
#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"
#include "Bang/ShaderProgramFactory.h"
#include "Bang/Transform.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/NotSelectableInEditor.h"

namespace Bang
{
class ShaderProgram;
}

using namespace Bang;
using namespace BangEditor;

EditorFloor::EditorFloor()
{
    AddComponent<Transform>();
    m_lineRenderer = AddComponent<LineRenderer>();
    m_lineRenderer->SetMaterial(MaterialFactory::GetDefaultUnLighted().Get());
    m_lineRenderer->GetMaterial()->SetAlbedoColor(Color::White);
    m_lineRenderer->GetMaterial()->SetReceivesLighting(false);
    m_lineRenderer->GetMaterial()->SetLineWidth(2.0f);
    m_lineRenderer->SetReceivesShadows(false);
    m_lineRenderer->SetCastsShadows(false);

    ShaderProgram *sp = ShaderProgramFactory::Get(
        ShaderProgramFactory::GetDefaultVertexShaderPath(),
        EditorPaths::GetEditorAssetsDir().Append("Shaders").Append(
            "EditorFloor.frag"));
    m_lineRenderer->GetMaterial()->SetRenderPass(RenderPass::OVERLAY);
    m_lineRenderer->GetMaterial()->SetShaderProgram(sp);

    Array<Vector3> floorLinePoints;
    constexpr float GridSize = 1500.0f;
    constexpr int GridNumCells = 100;
    constexpr float HGS = GridSize / 2;
    constexpr float CellSize = (GridSize / GridNumCells);
    for(int i = 0; i < GridNumCells; ++i)
    {
        {
            Vector3 lineBegin = Vector3(-HGS + CellSize * i, 0, -HGS);
            Vector3 lineEnd = Vector3(-HGS + CellSize * i, 0, HGS);
            floorLinePoints.PushBack(lineBegin);
            floorLinePoints.PushBack(lineEnd);
        }
        {
            Vector3 lineBegin = Vector3(-HGS, 0.0f, -HGS + CellSize * i);
            Vector3 lineEnd = Vector3(HGS, 0.0f, -HGS + CellSize * i);
            floorLinePoints.PushBack(lineBegin);
            floorLinePoints.PushBack(lineEnd);
        }
    }
    m_lineRenderer->SetPoints(floorLinePoints);

    AddComponent<HideInHierarchy>();
    AddComponent<NotSelectableInEditor>();
}

EditorFloor::~EditorFloor()
{
}

void EditorFloor::Render(RenderPass renderPass, bool renderChildren)
{
    if(renderPass == RenderPass::OVERLAY)
    {
        GBuffer *gbuffer = GEngine::GetActiveGBuffer();
        gbuffer->PushDepthStencilTexture();
        gbuffer->SetSceneDepthStencil();

        GameObject::Render(renderPass, renderChildren);

        gbuffer->PopDepthStencilTexture();
    }
}
