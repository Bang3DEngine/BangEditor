#include "BangEditor/FIWModel.h"

#include "Bang/RectTransform.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIAspectRatioFitter.h"
#include "Bang/UIContentSizeFitter.h"

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

    p_modelPreviewImg = modelPreviewGo->AddComponent<UIImageRenderer>();
    p_modelPreviewImg->SetMode(UIImageRenderer::Mode::TEXTURE);
    p_modelPreviewImg->SetImageTexture( TextureFactory::GetWhiteTexture().Get() );

    UIContentSizeFitter *previewContentSizeFitter =
                     modelPreviewGo->AddComponent<UIContentSizeFitter>();
    previewContentSizeFitter->SetVerticalSizeType(LayoutSizeType::PREFERRED);
    previewContentSizeFitter->SetHorizontalSizeType(LayoutSizeType::PREFERRED);

    UIAspectRatioFitter *previewAspectRatioSizeFitter =
                     modelPreviewGo->AddComponent<UIAspectRatioFitter>();
    previewAspectRatioSizeFitter->SetAspectRatio(1.0f);
    previewAspectRatioSizeFitter->SetAspectRatioMode(AspectRatioMode::KEEP);

    AddWidget(modelPreviewGo, 256);

    SetLabelsWidth(130);
}

Model *FIWModel::GetModel() const
{
    return SCAST<Model*>(GetResource().Get());
}

void FIWModel::UpdateInputsFromResource()
{
    p_modelPreviewImg->SetImageTexture(
            ModelPreviewFactory::GetPreviewTextureFor(GetModel()).Get() );
}

void FIWModel::OnValueChangedFIWResource(EventEmitter<IEventsValueChanged>*)
{
    if (!GetModel()) { return; }
}

