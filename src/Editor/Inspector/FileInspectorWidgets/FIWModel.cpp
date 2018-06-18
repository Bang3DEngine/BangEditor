#include "BangEditor/FIWModel.h"

#include "Bang/RectTransform.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIAspectRatioFitter.h"
#include "Bang/UIContentSizeFitter.h"

#include "BangEditor/PreviewViewer.h"
#include "BangEditor/ModelPreviewFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

FIWModel::FIWModel()
{
}

FIWModel::~FIWModel()
{
}

void FIWModel::Init()
{
    FileInspectorWidget::Init();

    SetName("FIWModel");
    SetTitle("Model");

    GameObject *modelPreviewGo = GameObjectFactory::CreateUIGameObject();
    modelPreviewGo->GetRectTransform()->SetAnchors(Vector2::Zero);
    modelPreviewGo->GetRectTransform()->SetPivotPosition(Vector2::Zero);

    p_modelPreviewViewer = GameObject::Create<PreviewViewer>();

    AddWidget(p_modelPreviewViewer, 256);

    SetLabelsWidth(130);
}

Model *FIWModel::GetModel() const
{
    return SCAST<Model*>(GetResource().Get());
}

void FIWModel::UpdateInputsFromResource()
{
    p_modelPreviewViewer->SetPreviewImageProvider([this](
                      const ResourcePreviewFactoryParameters &params)
    {
        return ModelPreviewFactory::GetPreviewTextureFor(GetModel(), params);
    });
}

void FIWModel::OnValueChangedFIWResource(EventEmitter<IEventsValueChanged>*)
{
    if (!GetModel()) { return; }
}

