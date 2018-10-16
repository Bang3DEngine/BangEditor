#include "BangEditor/UISceneToolbarDown.h"

#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/Stretch.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UILabel.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UISlider.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/EditorCamera.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UISceneToolbarDown::UISceneToolbarDown()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hLayout = AddComponent<UIHorizontalLayout>();
    hLayout->SetChildrenHorizontalStretch(Stretch::NONE);

    UILabel *zoomSpeedLabel = GameObjectFactory::CreateUILabel();
    zoomSpeedLabel->GetText()->SetContent("Zoom speed:");
    zoomSpeedLabel->GetGameObject()->SetParent(this);

    UILayoutElement *labelLE = zoomSpeedLabel->GetGameObject()->
                               AddComponent<UILayoutElement>();
    labelLE->SetFlexibleWidth(0.0f);
    labelLE->SetLayoutPriority(2);

    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::PREFERRED, 10)->SetParent(this);

    p_zoomSpeedSlider = GameObjectFactory::CreateUISlider();
    p_zoomSpeedSlider->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_zoomSpeedSlider->SetMinMaxValues(0.01f, 3.0f);
    p_zoomSpeedSlider->SetValue(1.0f);
    p_zoomSpeedSlider->GetInputNumber()->GetGameObject()->SetEnabled(false);
    p_zoomSpeedSlider->GetGameObject()->SetParent(this);

    UILayoutElement *zoomSpeedSliderLE = p_zoomSpeedSlider->GetGameObject()->
                                         AddComponent<UILayoutElement>();
    zoomSpeedSliderLE->SetMinWidth(50);
    zoomSpeedSliderLE->SetPreferredWidth(100);
    zoomSpeedSliderLE->SetFlexibleWidth(0.0f);
    zoomSpeedSliderLE->SetLayoutPriority(5);

    constexpr int ToolBarHeight = 20;
    UILayoutElement *toolbarLE = AddComponent<UILayoutElement>();
    toolbarLE->SetPreferredHeight(ToolBarHeight);
    toolbarLE->SetFlexibleWidth( 1.0f );
}

UISceneToolbarDown::~UISceneToolbarDown()
{
}

void UISceneToolbarDown::OnValueChanged(EventEmitter<IEventsValueChanged>*)
{
    float zoomSpeed = p_zoomSpeedSlider->GetValue();
    zoomSpeed *= zoomSpeed; // Quadratic slider value
    EditorCamera::GetInstance()->SetZoomSpeedMultiplier(zoomSpeed);
}

