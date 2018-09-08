#include "BangEditor/PreviewViewer.h"

#include "Bang/Input.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIAspectRatioFitter.h"
#include "Bang/UIContentSizeFitter.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

PreviewViewer::PreviewViewer()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    GetRectTransform()->SetAnchors(Vector2::Zero);
    GetRectTransform()->SetPivotPosition(Vector2::Zero);

    p_imgRenderer = AddComponent<UIImageRenderer>();
    p_imgRenderer->SetMode(UIImageRenderer::Mode::TEXTURE);
    p_imgRenderer->SetImageTexture( TextureFactory::GetWhiteTexture() );
    GameObjectFactory::AddInnerShadow(this, Vector2i(20));

    UIContentSizeFitter *previewContentSizeFitter =
                                        AddComponent<UIContentSizeFitter>();
    previewContentSizeFitter->SetVerticalSizeType(LayoutSizeType::PREFERRED);
    previewContentSizeFitter->SetHorizontalSizeType(LayoutSizeType::PREFERRED);

    UIAspectRatioFitter *previewAspectRatioSizeFitter =
                                        AddComponent<UIAspectRatioFitter>();
    previewAspectRatioSizeFitter->SetAspectRatio(1.0f);
    previewAspectRatioSizeFitter->SetAspectRatioMode(AspectRatioMode::KEEP);

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->AddEventCallback([this](IFocusable*, const UIEvent &event)
    {
        switch (event.type)
        {
            // case UIEvent::Type::MOUSE_WHEEL:
            // break;
            default: break;
        }
        return UIEventResult::IGNORE;
    });
}

PreviewViewer::~PreviewViewer()
{
}

void PreviewViewer::Update()
{
    GameObject::Update();

    if (p_focusable->IsBeingPressed())
    {
        constexpr float RotationSpeed = 3.0f;
        m_params.camOrbitAnglesDegs +=
                        Vector2(Input::GetMouseDelta()) * RotationSpeed;
        m_params.camOrbitAnglesDegs.y = Math::Clamp(m_params.camOrbitAnglesDegs.y,
                                                    -75.0f, 75.0f);
    }

    if (m_previewImageProviderFunc)
    {
        p_imgRenderer->SetImageTexture(
                            m_previewImageProviderFunc(m_params).Get() );
    }
}

void PreviewViewer::SetPreviewImageProvider(
                            PreviewViewer::ImageProviderFunc previewImgProvider)
{
    m_previewImageProviderFunc = previewImgProvider;
}

