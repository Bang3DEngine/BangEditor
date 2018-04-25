#include "BangEditor/UISceneToolbar.h"

#include <functional>

#include "Bang/UIButton.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/IconManager.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneImage.h"
#include "BangEditor/TransformGizmo.h"
#include "BangEditor/EditorIconManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UISceneToolbar::UISceneToolbar()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *toolbarHL = AddComponent<UIHorizontalLayout>();
    toolbarHL->SetSpacing(6);

    constexpr int ToolBarHeight = 20;

    UILayoutElement *toolbarLE = AddComponent<UILayoutElement>();
    toolbarLE->SetPreferredHeight(ToolBarHeight);
    toolbarLE->SetFlexibleWidth( 1.0f );

    RH<Texture2D> rightArrowIcon       = IconManager::GetRightArrowIcon();
    RH<Texture2D> doubleBarIcon        = EditorIconManager::GetDoubleBarIcon();
    RH<Texture2D> squareIcon           = EditorIconManager::GetSquareIcon();
    RH<Texture2D> rightArrowAndBarIcon = EditorIconManager::GetRightArrowAndBarIcon();
    RH<Texture2D> translateIcon        = EditorIconManager::GetHairCrossIcon();
    RH<Texture2D> rotateIcon           = EditorIconManager::GetRotateIcon();
    RH<Texture2D> scaleIcon            = EditorIconManager::GetAxesIcon();
    RH<Texture2D> rectTransformIcon    = EditorIconManager::GetAnchoredRectIcon();

    auto AddToolbarButton = [&](UIButton **button, Texture2D *icon,
                                std::function<void(IFocusable*)> callbackFunc)
    {
        (*button) = GameObjectFactory::CreateUIButton("", icon);
        (*button)->SetIcon(icon, Vector2i(ToolBarHeight));
        (*button)->GetLayoutElement()->SetMinSize( Vector2i(ToolBarHeight) );
        (*button)->GetIcon()->SetTint(Color::DarkGray);
        (*button)->GetFocusable()->AddClickedCallback(callbackFunc);
        (*button)->GetGameObject()->SetParent(this);
    };

    AddToolbarButton(&p_translateButton, translateIcon.Get(),
         [&](IFocusable*) { TransformGizmo::GetInstance()->SetTransformMode(
                        TransformGizmo::TransformMode::Translate); });
    AddToolbarButton(&p_rotateButton, rotateIcon.Get(),
         [&](IFocusable*) { TransformGizmo::GetInstance()->SetTransformMode(
                        TransformGizmo::TransformMode::Rotate); });
    AddToolbarButton(&p_scaleButton, scaleIcon.Get(),
         [&](IFocusable*) { TransformGizmo::GetInstance()->SetTransformMode(
                        TransformGizmo::TransformMode::Scale); });
    AddToolbarButton(&p_rectTransformButton, rectTransformIcon.Get(),
         [&](IFocusable*) { TransformGizmo::GetInstance()->SetTransformMode(
                        TransformGizmo::TransformMode::Rect); });

    GameObjectFactory::CreateUIHSpacer()->SetParent(this);

    AddToolbarButton(&p_playButton, rightArrowIcon.Get(),
                     [&](IFocusable*) { ScenePlayer::PlayScene(); });
    AddToolbarButton(&p_pauseButton, doubleBarIcon.Get(),
                     [&](IFocusable*) { ScenePlayer::PauseScene(); });
    AddToolbarButton(&p_stepButton, rightArrowAndBarIcon.Get(),
                     [&](IFocusable*) { ScenePlayer::StepFrame(); });
    AddToolbarButton(&p_stopButton, squareIcon.Get(),
                     [&](IFocusable*) { ScenePlayer::StopScene(); });

    p_renderModeInput = GameObjectFactory::CreateUIComboBox();
    p_renderModeInput->AddItem("Color",            SCAST<int>(UISceneImage::RenderMode::Color));
    p_renderModeInput->AddItem("Albedo",           SCAST<int>(UISceneImage::RenderMode::Albedo));
    p_renderModeInput->AddItem("Normal",           SCAST<int>(UISceneImage::RenderMode::Normal));
    p_renderModeInput->AddItem("World Position",   SCAST<int>(UISceneImage::RenderMode::WorldPosition));
    p_renderModeInput->AddItem("Roughness",        SCAST<int>(UISceneImage::RenderMode::Roughness));
    p_renderModeInput->AddItem("Metalness",        SCAST<int>(UISceneImage::RenderMode::Metalness));
    p_renderModeInput->AddItem("Receives Light",   SCAST<int>(UISceneImage::RenderMode::ReceivesLight));
    p_renderModeInput->AddItem("Receives Shadows", SCAST<int>(UISceneImage::RenderMode::ReceivesShadows));
    p_renderModeInput->AddItem("Depth",            SCAST<int>(UISceneImage::RenderMode::Depth));
    p_renderModeInput->AddItem("Selection",        SCAST<int>(UISceneImage::RenderMode::Selection));
    p_renderModeInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_showDebugStatsCheckbox = GameObjectFactory::CreateUICheckBox();
    p_showDebugStatsCheckbox->SetChecked(false);
    p_showDebugStatsCheckbox->EventEmitter<IValueChangedListener>::RegisterListener(this);

    GameObject *showDebugStatsTextGo = GameObjectFactory::CreateUIGameObject();
    UITextRenderer*showDebugStatsText =
                        showDebugStatsTextGo->AddComponent<UITextRenderer>();
    showDebugStatsText->SetTextSize(11);
    showDebugStatsText->SetContent("Stats");

    GameObjectFactory::CreateUISpacer(LayoutSizeType::Flexible, Vector2::One)->
                        SetParent(this);
    p_renderModeInput->GetGameObject()->SetParent(this);
    showDebugStatsTextGo->SetParent(this);
    p_showDebugStatsCheckbox->GetGameObject()->SetParent(this);

    ScenePlayer::GetInstance()->EventEmitter<IScenePlayerListener>::RegisterListener(this);
}

UISceneToolbar::~UISceneToolbar()
{
}

void UISceneToolbar::Update()
{
    GameObject::Update();

    p_translateButton->SetBlocked(false);
    p_rotateButton->SetBlocked(false);
    p_scaleButton->SetBlocked(false);
    p_rectTransformButton->SetBlocked(false);

    GameObject *selGO = Editor::GetSelectedGameObject();
    p_translateButton->GetGameObject()->SetVisible(selGO && selGO->GetTransform());
    p_rotateButton->GetGameObject()->SetVisible(selGO && selGO->GetTransform());
    p_scaleButton->GetGameObject()->SetVisible(selGO && selGO->GetTransform());
    p_rectTransformButton->GetGameObject()->SetVisible(selGO && selGO->GetRectTransform());

    switch (TransformGizmo::GetInstance()->GetTransformMode())
    {
        case TransformGizmo::TransformMode::Translate:
            p_translateButton->SetBlocked(true);
        break;

        case TransformGizmo::TransformMode::Rotate:
            p_rotateButton->SetBlocked(true);
        break;

        case TransformGizmo::TransformMode::Scale:
            p_scaleButton->SetBlocked(true);
        break;

        case TransformGizmo::TransformMode::Rect:
            p_rectTransformButton->SetBlocked(true);
        break;
    }
}

bool UISceneToolbar::IsShowDebugStatsChecked() const
{
    return p_showDebugStatsCheckbox->IsChecked();
}

UIComboBox *UISceneToolbar::GetRenderModeComboBox() const
{
    return p_renderModeInput;
}

void UISceneToolbar::OnPlayScene()
{
    p_playButton->SetBlocked(true);
    p_pauseButton->SetBlocked(false);
    p_stepButton->SetBlocked(false);
    p_stopButton->SetBlocked(false);
}

void UISceneToolbar::OnPauseScene()
{
    p_playButton->SetBlocked(false);
    p_pauseButton->SetBlocked(true);
    p_stepButton->SetBlocked(false);
    p_stopButton->SetBlocked(false);
}

void UISceneToolbar::OnStopScene()
{
    p_playButton->SetBlocked(false);
    p_pauseButton->SetBlocked(true);
    p_stepButton->SetBlocked(true);
    p_stopButton->SetBlocked(true);
}

void UISceneToolbar::OnPlayStateChanged(PlayState,
                                        PlayState newPlayState)
{
    switch (newPlayState)
    {
        case PlayState::Editing:   OnStopScene();      break;
        case PlayState::Paused:    OnPauseScene();     break;
        case PlayState::Playing:   OnPlayScene();      break;
        default: break;
    }
}

void UISceneToolbar::OnValueChanged(Object *object)
{
    EventEmitter<IValueChangedListener>::PropagateToListeners(
                    &IValueChangedListener::OnValueChanged, object);
}
