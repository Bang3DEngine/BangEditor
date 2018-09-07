#include "BangEditor/CIWMeshRenderer.h"

#include "Bang/Mesh.h"
#include "Bang/UILabel.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UIInputText.h"
#include "Bang/MeshRenderer.h"
#include "Bang/UITextRenderer.h"

#include "BangEditor/UIInputFileWithPreview.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWMeshRenderer::InitInnerWidgets()
{
    CIWRenderer::InitInnerWidgets();

    SetName("CIWMeshRenderer");
    SetTitle("Mesh Renderer");

    p_meshInputFile = GameObject::Create<UIInputFileWithPreview>();
    p_meshInputFile->SetExtensions( { Extensions::GetMeshExtension() } );
    p_meshInputFile->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Mesh", p_meshInputFile);

    SetLabelsWidth(60);
}

void CIWMeshRenderer::UpdateFromReference()
{
    CIWRenderer::UpdateFromReference();

    Mesh *mesh = GetMeshRenderer()->GetSharedMesh();
    Path meshPath = mesh ? mesh->GetResourceFilepath() : Path::Empty;
    p_meshInputFile->SetPath(meshPath);
}

void CIWMeshRenderer::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    CIWRenderer::OnValueChangedCIW(object);

    if (object == p_meshInputFile)
    {
        Path meshPath = p_meshInputFile->GetPath();
        RH<Mesh> mesh = Resources::Load<Mesh>(meshPath);
        GetMeshRenderer()->SetMesh(mesh.Get());
    }
}

MeshRenderer *CIWMeshRenderer::GetMeshRenderer() const
{
    return SCAST<MeshRenderer*>( GetRenderer() );
}
