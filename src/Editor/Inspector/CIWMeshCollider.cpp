#include "BangEditor/CIWMeshCollider.h"

#include "Bang/Mesh.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/MeshCollider.h"

#include "BangEditor/UIInputFile.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWMeshCollider::CIWMeshCollider()
{
}

CIWMeshCollider::~CIWMeshCollider()
{
}

void CIWMeshCollider::InitInnerWidgets()
{
    CIWCollider::InitInnerWidgets();

    SetName("CIWMeshCollider");
    SetTitle("MeshCollider");

    p_meshInput = GameObject::Create<UIInputFile>();
    p_meshInput->SetExtensions( { Extensions::GetMeshExtension() } );
    p_meshInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Mesh", p_meshInput);

    SetLabelsWidth(95);
}

void CIWMeshCollider::UpdateFromReference()
{
    CIWCollider::UpdateFromReference();

    MeshCollider *meshCollider = GetMeshCollider();
    p_meshInput->SetPath( meshCollider->GetMesh() ?
                            meshCollider->GetMesh()->GetResourceFilepath() :
                            Path::Empty );
}

void CIWMeshCollider::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    CIWCollider::OnValueChangedCIW(object);

    MeshCollider *meshCollider = GetMeshCollider();
    meshCollider->SetMesh( Resources::Load<Mesh>(p_meshInput->GetPath()).Get() );
}

MeshCollider *CIWMeshCollider::GetMeshCollider() const
{
    return SCAST<MeshCollider*>( GetCollider() );
}

