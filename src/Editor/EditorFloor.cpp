#include "BangEditor/EditorFloor.h"

#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/LineRenderer.h"
#include "Bang/ShaderProgram.h"
#include "Bang/MaterialFactory.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/ShaderProgramFactory.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/NotSelectableInEditor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorFloor::EditorFloor()
{
    AddComponent<Transform>();
    m_lineRenderer = AddComponent<LineRenderer>();
    m_lineRenderer->SetMaterial( MaterialFactory::GetDefaultUnLighted().Get() );
    m_lineRenderer->GetMaterial()->SetAlbedoColor(Color::White);
    m_lineRenderer->GetMaterial()->SetReceivesLighting(false);
    m_lineRenderer->SetReceivesShadows(false);
    m_lineRenderer->SetCastsShadows(false);
    m_lineRenderer->SetLineWidth(2.0f);

    ShaderProgram *sp = ShaderProgramFactory::Get(
                ShaderProgramFactory::GetDefaultVertexShaderPath(),
                EditorPaths::GetEditorAssetsDir().Append("Shaders").
                    Append("EditorFloor.frag"));
    m_lineRenderer->GetMaterial()->SetShaderProgram(sp);

    Array<Vector3> floorLinePoints;
    constexpr float GridSize = 15000.0f;
    constexpr int GridNumCells = 1000;
    constexpr float HGS = GridSize / 2;
    constexpr float CellSize = (GridSize / GridNumCells);
    for (int i = 0; i < GridNumCells; ++i)
    {
        {
        Vector3 lineBegin = Vector3(-HGS + CellSize * i, 0, -HGS);
        Vector3 lineEnd   = Vector3(-HGS + CellSize * i, 0,  HGS);
        floorLinePoints.PushBack(lineBegin);
        floorLinePoints.PushBack(lineEnd);
        }
        {
        Vector3 lineBegin = Vector3(-HGS, 0.0f, -HGS + CellSize * i);
        Vector3 lineEnd   = Vector3( HGS, 0.0f, -HGS + CellSize * i);
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
    // GBuffer *gbuffer = GEngine::GetActiveGBuffer();
    // gbuffer->PushDepthStencilTexture();
    // gbuffer->SetSceneDepthStencil();

    GameObject::Render(renderPass, renderChildren);

    // gbuffer->PopDepthStencilTexture();
}

