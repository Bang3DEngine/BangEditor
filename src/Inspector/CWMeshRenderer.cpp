#include "BangEditor/CWMeshRenderer.h"

#include "Bang/Mesh.h"
#include "Bang/UILabel.h"
#include "Bang/UIInputText.h"
#include "Bang/MeshRenderer.h"
#include "Bang/UITextRenderer.h"

#include "BangEditor/UIInputFile.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CWMeshRenderer::CWMeshRenderer()
{
    SetName("CWMeshRenderer");
    SetTitle("Mesh Renderer");

    p_meshInputFile = GameObject::Create<UIInputFile>();
    p_meshInputFile->GetLabel()->GetText()->SetContent("Mesh");
    p_meshInputFile->SetParent( GetContainer() );
}

CWMeshRenderer::~CWMeshRenderer()
{
}

void CWMeshRenderer::Update()
{
    ComponentWidget::Update();

    if (!p_meshInputFile->GetInputText()->GetLabel()->HasFocus())
    {
        Mesh *mesh = p_relatedMeshRenderer->GetSharedMesh();
        Path meshPath = mesh ? mesh->GetResourceFilepath() : Path::Empty;
        p_meshInputFile->GetInputText()->GetText()->SetContent(meshPath.GetNameExt());
    }
}

void CWMeshRenderer::OnValueChanged(Object *object)
{

}

void CWMeshRenderer::SetComponent(Component *comp)
{
    MeshRenderer *meshRenderer = DCAST<MeshRenderer*>(comp);
    ASSERT(meshRenderer);

    p_relatedMeshRenderer = meshRenderer;
}

