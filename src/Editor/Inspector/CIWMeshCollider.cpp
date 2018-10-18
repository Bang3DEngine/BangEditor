#include "BangEditor/CIWMeshCollider.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/IEvents.h"
#include "Bang/Mesh.h"
#include "Bang/MeshCollider.h"
#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
#include "BangEditor/UIInputFile.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

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

    p_meshInput = new UIInputFile();
    p_meshInput->SetExtensions({Extensions::GetMeshExtension()});
    p_meshInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Mesh", p_meshInput);

    SetLabelsWidth(95);
}

void CIWMeshCollider::UpdateFromReference()
{
    CIWCollider::UpdateFromReference();

    MeshCollider *meshCollider = GetMeshCollider();
    p_meshInput->SetPath(meshCollider->GetMesh()
                             ? meshCollider->GetMesh()->GetResourceFilepath()
                             : Path::Empty);
}

void CIWMeshCollider::OnValueChangedCIW(
    EventEmitter<IEventsValueChanged> *object)
{
    CIWCollider::OnValueChangedCIW(object);

    MeshCollider *meshCollider = GetMeshCollider();
    meshCollider->SetMesh(Resources::Load<Mesh>(p_meshInput->GetPath()).Get());
}

MeshCollider *CIWMeshCollider::GetMeshCollider() const
{
    return SCAST<MeshCollider *>(GetCollider());
}
