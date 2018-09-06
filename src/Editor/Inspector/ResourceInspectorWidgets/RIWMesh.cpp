#include "BangEditor/RIWMesh.h"

#include "Bang/RectTransform.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIAspectRatioFitter.h"
#include "Bang/UIContentSizeFitter.h"

#include "BangEditor/PreviewViewer.h"
#include "BangEditor/MeshPreviewFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RIWMesh::RIWMesh()
{
}

RIWMesh::~RIWMesh()
{
}

void RIWMesh::Init()
{
    ResourceInspectorWidget::Init();

    SetName("RIWMesh");
    SetTitle("Mesh");

    GameObject *MeshPreviewGo = GameObjectFactory::CreateUIGameObject();
    MeshPreviewGo->GetRectTransform()->SetAnchors(Vector2::Zero);
    MeshPreviewGo->GetRectTransform()->SetPivotPosition(Vector2::Zero);

    p_meshPreviewViewer = GameObject::Create<PreviewViewer>();

    AddWidget(p_meshPreviewViewer, 256);

    SetLabelsWidth(130);
}

Mesh *RIWMesh::GetMesh() const
{
    return SCAST<Mesh*>(GetResource().Get());
}

void RIWMesh::UpdateInputsFromResource()
{
    p_meshPreviewViewer->SetPreviewImageProvider([this](
                      const ResourcePreviewFactoryParameters &params)
    {
        return MeshPreviewFactory::GetPreviewTextureFor(GetMesh(), params);
    });
}

Texture2D* RIWMesh::GetIconTexture() const
{
    return MeshPreviewFactory::GetPreviewTextureFor( GetMesh() ).Get();
}

void RIWMesh::OnValueChangedRIWResource(EventEmitter<IEventsValueChanged>*)
{
    if (!GetMesh())
    {
        return;
    }
}

