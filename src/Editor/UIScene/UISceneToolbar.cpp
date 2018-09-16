#include "BangEditor/UISceneToolbar.h"

#include <functional>

#include "Bang/UIButton.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/RectTransform.h"
#include "Bang/TextureFactory.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneImage.h"
#include "BangEditor/TransformGizmo.h"
#include "BangEditor/UISceneEditContainer.h"
#include "BangEditor/EditorTextureFactory.h"

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

    Texture2D *rightArrowIcon       = TextureFactory::GetRightArrowIcon();
    Texture2D *doubleBarIcon        = EditorTextureFactory::GetDoubleBarIcon();
    Texture2D *squareIcon           = EditorTextureFactory::GetSquareIcon();
    Texture2D *rightArrowAndBarIcon = EditorTextureFactory::GetRightArrowAndBarIcon();
    Texture2D *translateIcon        = EditorTextureFactory::GetHairCrossIcon();
    Texture2D *rotateIcon           = EditorTextureFactory::GetRotateIcon();
    Texture2D *scaleIcon            = EditorTextureFactory::GetAxesIcon();
    Texture2D *rectTransformIcon    = EditorTextureFactory::GetAnchoredRectIcon();

    auto AddToolbarButton = [&](UIButton **button,
                                Texture2D *icon,
                                std::function<void()> callbackFunc)
    {
        (*button) = GameObjectFactory::CreateUIButton("", icon);
        (*button)->SetIcon(icon, Vector2i(ToolBarHeight));
        (*button)->GetLayoutElement()->SetMinSize( Vector2i(ToolBarHeight) );
        (*button)->GetIcon()->SetTint(Color::DarkGray);
        (*button)->AddClickedCallback( [callbackFunc]() { callbackFunc(); });
        (*button)->GetGameObject()->SetParent(this);
    };

    AddToolbarButton(&p_translateButton, translateIcon,
                     [&]() { SetTransformGizmoMode( TransformGizmoMode::TRANSLATE); });
    AddToolbarButton(&p_rotateButton, rotateIcon,
                     [&]() { SetTransformGizmoMode( TransformGizmoMode::ROTATE); });
    AddToolbarButton(&p_scaleButton, scaleIcon,
                     [&]() { SetTransformGizmoMode( TransformGizmoMode::SCALE); });
    AddToolbarButton(&p_rectTransformButton, rectTransformIcon,
                     [&]() { SetTransformGizmoMode( TransformGizmoMode::RECT); });

    GameObjectFactory::CreateUIHSpacer()->SetParent(this);

    AddToolbarButton(&p_playButton, rightArrowIcon,
                     [&]() { ScenePlayer::PlayScene(); });
    AddToolbarButton(&p_pauseButton, doubleBarIcon,
                     [&]() { ScenePlayer::PauseScene(); });
    AddToolbarButton(&p_stepButton, rightArrowAndBarIcon,
                     [&]() { ScenePlayer::StepFrame(); });
    AddToolbarButton(&p_stopButton, squareIcon,
                     [&]() { ScenePlayer::StopScene(); });

    p_renderModeInput = GameObjectFactory::CreateUIComboBox();
    p_renderModeInput->AddItem("Color",            SCAST<int>(UISceneImage::RenderMode::COLOR));
    p_renderModeInput->AddItem("Albedo",           SCAST<int>(UISceneImage::RenderMode::ALBEDO));
    p_renderModeInput->AddItem("Normal",           SCAST<int>(UISceneImage::RenderMode::NORMAL));
    p_renderModeInput->AddItem("World Position",   SCAST<int>(UISceneImage::RenderMode::WORLD_POSITION));
    p_renderModeInput->AddItem("Roughness",        SCAST<int>(UISceneImage::RenderMode::ROUGHNESS));
    p_renderModeInput->AddItem("Metalness",        SCAST<int>(UISceneImage::RenderMode::METALNESS));
    p_renderModeInput->AddItem("Receives Light",   SCAST<int>(UISceneImage::RenderMode::RECEIVES_LIGHT));
    p_renderModeInput->AddItem("Receives Shadows", SCAST<int>(UISceneImage::RenderMode::RECEIVES_SHADOWS));
    p_renderModeInput->AddItem("Depth",            SCAST<int>(UISceneImage::RenderMode::DEPTH));
    p_renderModeInput->AddItem("Selection",        SCAST<int>(UISceneImage::RenderMode::SELECTION));
    p_renderModeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_showDebugStatsCheckbox = GameObjectFactory::CreateUICheckBox();
    p_showDebugStatsCheckbox->SetChecked(false);
    p_showDebugStatsCheckbox->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    GameObject *showDebugStatsTextGo = GameObjectFactory::CreateUIGameObject();
    UITextRenderer*showDebugStatsText =
                        showDebugStatsTextGo->AddComponent<UITextRenderer>();
    showDebugStatsText->SetTextSize(11);
    showDebugStatsText->SetContent("Stats");

    GameObjectFactory::CreateUISpacer(LayoutSizeType::FLEXIBLE, Vector2::One)->
                        SetParent(this);
    p_renderModeInput->GetGameObject()->SetParent(this);
    showDebugStatsTextGo->SetParent(this);
    p_showDebugStatsCheckbox->GetGameObject()->SetParent(this);

    OnStopScene();
    ScenePlayer::GetInstance()->EventEmitter<IEventsScenePlayer>::RegisterListener(this);
}

UISceneToolbar::~UISceneToolbar()
{
}

void UISceneToolbar::Update()
{
    GameObject::Update();

    GameObject *selGO = Editor::GetSelectedGameObject();
    if (selGO)
    {
        if (selGO->HasComponent<RectTransform>())
        {
            SetTransformGizmoMode(TransformGizmoMode::RECT);
        }
        else if (GetTransformGizmoMode() == TransformGizmoMode::RECT)
        {
            SetTransformGizmoMode(TransformGizmoMode::TRANSLATE);
        }
    }

    p_translateButton->GetGameObject()->SetVisible(selGO && selGO->GetTransform());
    p_rotateButton->GetGameObject()->SetVisible(selGO && selGO->GetTransform());
    p_scaleButton->GetGameObject()->SetVisible(selGO && selGO->GetTransform());
    p_rectTransformButton->GetGameObject()->SetVisible(selGO && selGO->GetRectTransform());

    switch (GetTransformGizmoMode())
    {
        case TransformGizmoMode::TRANSLATE:
            p_translateButton->SetBlocked(true);
            p_rotateButton->SetBlocked(false);
            p_scaleButton->SetBlocked(false);
            p_rectTransformButton->SetBlocked(false);
        break;

        case TransformGizmoMode::ROTATE:
            p_translateButton->SetBlocked(false);
            p_rotateButton->SetBlocked(true);
            p_scaleButton->SetBlocked(false);
            p_rectTransformButton->SetBlocked(false);
        break;

        case TransformGizmoMode::SCALE:
            p_translateButton->SetBlocked(false);
            p_rotateButton->SetBlocked(false);
            p_scaleButton->SetBlocked(true);
            p_rectTransformButton->SetBlocked(false);
        break;

        case TransformGizmoMode::RECT:
            p_translateButton->SetBlocked(false);
            p_rotateButton->SetBlocked(false);
            p_scaleButton->SetBlocked(false);
            p_rectTransformButton->SetBlocked(true);
        break;
    }
}

void UISceneToolbar::SetTransformGizmoMode(TransformGizmoMode transformMode)
{
    if (transformMode != GetTransformGizmoMode())
    {
        m_transformGizmoMode = transformMode;
    }
}

TransformGizmoMode UISceneToolbar::GetTransformGizmoMode() const
{
    return m_transformGizmoMode;
}

bool UISceneToolbar::IsShowDebugStatsChecked() const
{
    return p_showDebugStatsCheckbox->IsChecked();
}

UIComboBox *UISceneToolbar::GetRenderModeComboBox() const
{
    return p_renderModeInput;
}

UISceneToolbar *UISceneToolbar::GetActive()
{
    return UISceneEditContainer::GetActive()->GetSceneToolbar();
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
        case PlayState::EDITING:   OnStopScene();      break;
        case PlayState::PAUSED:    OnPauseScene();     break;
        case PlayState::PLAYING:   OnPlayScene();      break;
        default: break;
    }
}

void UISceneToolbar::OnValueChanged(EventEmitter<IEventsValueChanged> *object)
{
    EventEmitter<IEventsValueChanged>::PropagateToListeners(
                    &IEventsValueChanged::OnValueChanged, object);
}
