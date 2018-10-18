#include "BangEditor/RIWModel.h"

#include <sys/types.h>
#include <cstddef>
#include <functional>
#include <memory>

#include "Bang/Array.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/Map.h"
#include "Bang/Map.tcc"
#include "Bang/Mesh.h"
#include "Bang/RectTransform.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Texture2D.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIInputText.h"
#include "BangEditor/ModelPreviewFactory.h"
#include "BangEditor/PreviewViewer.h"
#include "BangEditor/RIWResource.tcc"
#include "BangEditor/ResourceInspectorWidget.h"

namespace Bang
{
class IEventsValueChanged;
template <class>
class EventEmitter;
}

namespace BangEditor
{
struct ResourcePreviewFactoryParameters;
}

using namespace Bang;
using namespace BangEditor;

RIWModel::RIWModel()
{
}

RIWModel::~RIWModel()
{
}

void RIWModel::Init()
{
    ResourceInspectorWidget::Init();

    SetName("RIWModel");
    SetTitle("Model");

    GameObject *modelPreviewGo = GameObjectFactory::CreateUIGameObject();
    modelPreviewGo->GetRectTransform()->SetAnchors(Vector2::Zero);
    modelPreviewGo->GetRectTransform()->SetPivotPosition(Vector2::Zero);

    p_modelPreviewViewer = new PreviewViewer();

    p_numMeshes = GameObjectFactory::CreateUIInputNumber();
    p_numMeshes->SetDecimalPlaces(0);
    p_numMeshes->GetInputText()->SetBlocked(true);

    p_numVertices = GameObjectFactory::CreateUIInputNumber();
    p_numVertices->SetDecimalPlaces(0);
    p_numVertices->GetInputText()->SetBlocked(true);

    p_numTriangles = GameObjectFactory::CreateUIInputNumber();
    p_numTriangles->SetDecimalPlaces(0);
    p_numTriangles->GetInputText()->SetBlocked(true);

    p_numBones = GameObjectFactory::CreateUIInputNumber();
    p_numBones->SetDecimalPlaces(0);
    p_numBones->GetInputText()->SetBlocked(true);

    p_numMaterials = GameObjectFactory::CreateUIInputNumber();
    p_numMaterials->SetDecimalPlaces(0);
    p_numMaterials->GetInputText()->SetBlocked(true);

    p_numAnimations = GameObjectFactory::CreateUIInputNumber();
    p_numAnimations->SetDecimalPlaces(0);
    p_numAnimations->GetInputText()->SetBlocked(true);

    AddWidget(p_modelPreviewViewer, 256);
    AddWidget(
        GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5));
    AddWidget("Num meshes", p_numMeshes->GetGameObject());
    AddWidget("Num vertices", p_numVertices->GetGameObject());
    AddWidget("Num triangles", p_numTriangles->GetGameObject());
    AddWidget("Num bones", p_numBones->GetGameObject());
    AddWidget(
        GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5));
    AddWidget("Num materials", p_numMaterials->GetGameObject());
    AddWidget(
        GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5));
    AddWidget("Num animations", p_numAnimations->GetGameObject());

    SetLabelsWidth(130);
}

Model *RIWModel::GetModel() const
{
    return SCAST<Model *>(GetResource().Get());
}

void RIWModel::OnResourceSet()
{
    RIWResource<Model>::OnResourceSet();
    std::size_t numMeshes = GetModel()->GetMeshes().Size();
    std::size_t numBones = GetModel()->GetAllBones().Size();
    std::size_t numMaterials = GetModel()->GetMaterials().Size();
    std::size_t numAnimations = GetModel()->GetAnimations().Size();

    std::size_t numVertices = 0;
    std::size_t numTriangles = 0;
    for (std::size_t i = 0; i < GetModel()->GetMeshes().Size(); ++i)
    {
        Mesh *mesh = GetModel()->GetMeshes()[i].Get();
        numVertices += static_cast<uint>(mesh->GetNumVertices());
        numTriangles += static_cast<uint>(mesh->GetNumTriangles());
    }

    p_numMeshes->SetValue(numMeshes);
    p_numBones->SetValue(numBones);
    p_numMaterials->SetValue(numMaterials);
    p_numAnimations->SetValue(numAnimations);
    p_numVertices->SetValue(numVertices);
    p_numTriangles->SetValue(numTriangles);
}

void RIWModel::UpdateInputsFromResource()
{
    p_modelPreviewViewer->SetPreviewImageProvider([this](
        const ResourcePreviewFactoryParameters &params) {
        return ModelPreviewFactory::GetPreviewTextureFor(GetModel(), params);
    });
}

Texture2D *RIWModel::GetIconTexture() const
{
    return ModelPreviewFactory::GetPreviewTextureFor(GetModel()).Get();
}

void RIWModel::OnValueChangedRIWResource(EventEmitter<IEventsValueChanged> *)
{
}
