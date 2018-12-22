#include "BangEditor/AIWMesh.h"

#include <functional>

#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/Map.h"
#include "Bang/Map.tcc"
#include "Bang/RectTransform.h"
#include "Bang/AssetHandle.h"
#include "Bang/Texture2D.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIInputText.h"
#include "BangEditor/MeshPreviewFactory.h"
#include "BangEditor/PreviewViewer.h"
#include "BangEditor/AIWAsset.tcc"
#include "BangEditor/AssetInspectorWidget.h"

namespace Bang
{
class IEventsValueChanged;
template <class>
class EventEmitter;
}  // namespace Bang

namespace BangEditor
{
struct AssetPreviewFactoryParameters;
}

using namespace Bang;
using namespace BangEditor;

AIWMesh::AIWMesh()
{
}

AIWMesh::~AIWMesh()
{
}

void AIWMesh::Init()
{
    AssetInspectorWidget::Init();

    SetName("AIWMesh");
    SetTitle("Mesh");

    GameObject *meshPreviewGo = GameObjectFactory::CreateUIGameObject();
    meshPreviewGo->GetRectTransform()->SetAnchors(Vector2::Zero());
    meshPreviewGo->GetRectTransform()->SetPivotPosition(Vector2::Zero());

    p_meshPreviewViewer = new PreviewViewer();

    p_numVertices = GameObjectFactory::CreateUIInputNumber();
    p_numVertices->SetDecimalPlaces(0);
    p_numVertices->GetInputText()->SetBlocked(true);

    p_numTriangles = GameObjectFactory::CreateUIInputNumber();
    p_numTriangles->SetDecimalPlaces(0);
    p_numTriangles->GetInputText()->SetBlocked(true);

    p_numBones = GameObjectFactory::CreateUIInputNumber();
    p_numBones->SetDecimalPlaces(0);
    p_numBones->GetInputText()->SetBlocked(true);

    AddWidget(p_meshPreviewViewer, 256);
    AddWidget(
        GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5));
    AddWidget("Num vertices", p_numVertices->GetGameObject());
    AddWidget("Num triangles", p_numTriangles->GetGameObject());
    AddWidget("Num bones", p_numBones->GetGameObject());

    SetLabelsWidth(130);
}

Mesh *AIWMesh::GetMesh() const
{
    return SCAST<Mesh *>(GetAsset().Get());
}

void AIWMesh::OnAssetSet()
{
    AIWAsset<Mesh>::OnAssetSet();
    p_numVertices->SetValue(SCAST<float>(GetMesh()->GetNumVertices()));
    p_numTriangles->SetValue(SCAST<float>(GetMesh()->GetNumTriangles()));
    p_numBones->SetValue(SCAST<float>(GetMesh()->GetBonesPool().Size()));
}

void AIWMesh::UpdateInputsFromAsset()
{
    p_meshPreviewViewer->SetPreviewImageProvider(
        [this](const AssetPreviewFactoryParameters &params) {
            return MeshPreviewFactory::GetPreviewTextureFor(GetMesh(), params);
        });
}

Texture2D *AIWMesh::GetIconTexture() const
{
    return MeshPreviewFactory::GetPreviewTextureFor(GetMesh()).Get();
}

void AIWMesh::OnValueChangedAIWAsset(EventEmitter<IEventsValueChanged> *)
{
    if (!GetMesh())
    {
        return;
    }
}
