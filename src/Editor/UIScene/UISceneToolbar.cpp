#include "BangEditor/UISceneToolbar.h"

#include <functional>

#include "Bang/UIButton.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIToolButton.h"
#include "Bang/RectTransform.h"
#include "Bang/TextureFactory.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/ScenePlayer.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/UISceneImage.h"
#include "BangEditor/TransformGizmo.h"
#include "BangEditor/EditorSceneManager.h"
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

    Texture2D *eyeIcon              = EditorTextureFactory::GetEyeIcon();
    Texture2D *rightArrowIcon       = TextureFactory::GetRightArrowIcon();
    Texture2D *doubleBarIcon        = EditorTextureFactory::GetDoubleBarIcon();
    Texture2D *squareIcon           = EditorTextureFactory::GetSquareIcon();
    Texture2D *rightArrowAndBarIcon = EditorTextureFactory::GetRightArrowAndBarIcon();
    Texture2D *translateIcon        = EditorTextureFactory::GetHairCrossIcon();
    Texture2D *rotateIcon           = EditorTextureFactory::GetRotateIcon();
    Texture2D *scaleIcon            = EditorTextureFactory::GetAxesIcon();
    Texture2D *rectTransformIcon    = EditorTextureFactory::GetAnchoredRectIcon();

    auto AddToolbarButton = [&](UIToolButton **buttonPtr,
                                Texture2D *icon,
                                std::function<void()> callbackFunc)
    {
        UIToolButton *button = GameObjectFactory::CreateUIToolButton("", icon);
        button->SetIcon(icon, Vector2i(ToolBarHeight));
        button->GetLayoutElement()->SetMinSize( Vector2i(ToolBarHeight) );
        button->GetIcon()->SetTint(Color::DarkGray);
        button->AddClickedCallback( [callbackFunc]() { callbackFunc(); } );
        button->GetGameObject()->SetParent(this);
        *buttonPtr = button;
    };
    auto AddTransformButton = [&](UIToolButton **buttonPtr,
                                  Texture2D *icon,
                                  std::function<void()> callbackFunc)
    {
        AddToolbarButton(buttonPtr, icon,
        [this, buttonPtr, callbackFunc]()
        {
            if ((*buttonPtr)->GetOn())
            {
                callbackFunc();
            }
            else
            {
                SetTransformGizmoMode( TransformGizmoMode::NONE );
            }
        } );
    };
    AddTransformButton(&p_translateButton, translateIcon,
                       [&]() { SetTransformGizmoMode( TransformGizmoMode::TRANSLATE); });
    AddTransformButton(&p_rotateButton, rotateIcon,
                       [&]() { SetTransformGizmoMode( TransformGizmoMode::ROTATE); });
    AddTransformButton(&p_scaleButton, scaleIcon,
                       [&]() { SetTransformGizmoMode( TransformGizmoMode::SCALE); });
    AddTransformButton(&p_rectTransformButton, rectTransformIcon,
                       [&]() { SetTransformGizmoMode( TransformGizmoMode::RECT); });

    p_transformCamSeparator = GameObjectFactory::CreateUIVSeparator(
                                                 LayoutSizeType::PREFERRED, 10);
    p_transformCamSeparator->SetParent(this);

    AddToolbarButton(&p_resetCamViewButton, eyeIcon,
                     [&]()
                     {
                         p_resetCamViewButton->SetOn(false);
                         ResetCameraView();
                     });

    p_transformCamSpacer = GameObjectFactory::CreateUIHSpacer();
    p_transformCamSpacer->SetParent(this);

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
    UITextRenderer *showDebugStatsText =
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
    UpdateToolButtons();
}

void UISceneToolbar::SetTransformGizmoMode(TransformGizmoMode transformMode)
{
    if (transformMode != GetTransformGizmoMode())
    {
        m_transformGizmoMode = transformMode;
        p_translateButton->SetOn( transformMode == TransformGizmoMode::TRANSLATE );
        p_rotateButton->SetOn( transformMode == TransformGizmoMode::ROTATE );
        p_scaleButton->SetOn( transformMode == TransformGizmoMode::SCALE );
        p_rectTransformButton->SetOn( transformMode == TransformGizmoMode::RECT );
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

void UISceneToolbar::DisableTransformAndCameraControls()
{
    p_transformCamSeparator->SetVisible(false);
    p_translateButton->GetGameObject()->SetVisible(false);
    p_rotateButton->GetGameObject()->SetVisible(false);
    p_scaleButton->GetGameObject()->SetVisible(false);
    p_rectTransformButton->GetGameObject()->SetVisible(false);
    p_resetCamViewButton->GetGameObject()->SetVisible(false);
}

UIComboBox *UISceneToolbar::GetRenderModeComboBox() const
{
    return p_renderModeInput;
}

UISceneToolbar *UISceneToolbar::GetActive()
{
    return UISceneEditContainer::GetActive()->GetSceneToolbar();
}

void UISceneToolbar::ResetCameraView()
{
    if (Scene *openScene = EditorSceneManager::GetActive()->GetOpenScene())
    {
        EditorCamera::GetInstance()->FocusScene(openScene);
    }
}

void UISceneToolbar::UpdateToolButtons()
{
    GameObject *selGo = Editor::GetSelectedGameObject();
    if (selGo)
    {
        bool hasRectTransform = selGo->HasComponent<RectTransform>();
        if (hasRectTransform)
        {
            SetTransformGizmoMode(TransformGizmoMode::RECT);
        }
        else if (GetTransformGizmoMode() == TransformGizmoMode::RECT)
        {
            SetTransformGizmoMode(TransformGizmoMode::TRANSLATE);
        }
    }

    p_translateButton->SetBlocked( !(selGo && selGo->GetTransform()) );
    p_rotateButton->SetBlocked( !(selGo && selGo->GetTransform()) );
    p_scaleButton->SetBlocked( !(selGo && selGo->GetTransform()) );
    p_rectTransformButton->SetBlocked( !(selGo && selGo->GetRectTransform()) );

    // W, E, R, T shortcuts handling
    TransformGizmoMode newTrMode = Undef<TransformGizmoMode>();
    if (Input::GetKeyDown(Key::W))
    {
        newTrMode = TransformGizmoMode::TRANSLATE;
    }
    else if (Input::GetKeyDown(Key::E))
    {
        newTrMode = TransformGizmoMode::ROTATE;
    }
    else if (Input::GetKeyDown(Key::R))
    {
        newTrMode = TransformGizmoMode::SCALE;
    }
    else if (Input::GetKeyDown(Key::T))
    {
        newTrMode = TransformGizmoMode::RECT;
    }

    if (newTrMode != Undef<TransformGizmoMode>())
    {
        SetTransformGizmoMode(newTrMode);
    }

    p_translateButton->SetOn( GetTransformGizmoMode() ==
                              TransformGizmoMode::TRANSLATE );
    p_rotateButton->SetOn( GetTransformGizmoMode() ==
                           TransformGizmoMode::ROTATE );
    p_scaleButton->SetOn( GetTransformGizmoMode() ==
                          TransformGizmoMode::SCALE );
    p_rectTransformButton->SetOn( GetTransformGizmoMode() ==
                                  TransformGizmoMode::RECT );
}

void UISceneToolbar::OnPlayScene()
{
    p_playButton->SetOn(true);
    p_pauseButton->SetOn(false);
    p_stepButton->SetOn(false);
    p_stopButton->SetOn(false);

    p_playButton->SetBlocked(true);
    p_pauseButton->SetBlocked(false);
    p_stepButton->SetBlocked(false);
    p_stopButton->SetBlocked(false);
}

void UISceneToolbar::OnPauseScene()
{
    p_playButton->SetOn(false);
    p_pauseButton->SetOn(true);
    p_stepButton->SetOn(false);
    p_stopButton->SetOn(false);

    p_playButton->SetBlocked(false);
    p_pauseButton->SetBlocked(true);
    p_stepButton->SetBlocked(false);
    p_stopButton->SetBlocked(false);
}

void UISceneToolbar::OnStopScene()
{
    p_playButton->SetOn(false);
    p_pauseButton->SetOn(true);
    p_stepButton->SetOn(true);
    p_stopButton->SetOn(true);

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
        case PlayState::EDITING:
            OnStopScene();
            p_playButton->SetBlocked(false);
            p_pauseButton->SetBlocked(true);
            p_stepButton->SetBlocked(true);
            p_stopButton->SetBlocked(true);
        break;

        case PlayState::PAUSED:
            OnPauseScene();
            p_playButton->SetBlocked(false);
            p_pauseButton->SetBlocked(true);
            p_stepButton->SetBlocked(false);
            p_stopButton->SetBlocked(false);
        break;

        case PlayState::PLAYING:
            OnPlayScene();
            p_playButton->SetBlocked(true);
            p_pauseButton->SetBlocked(false);
            p_stepButton->SetBlocked(false);
            p_stopButton->SetBlocked(false);
        break;

        default:
        break;
    }
}

void UISceneToolbar::OnValueChanged(EventEmitter<IEventsValueChanged> *object)
{
    EventEmitter<IEventsValueChanged>::PropagateToListeners(
                    &IEventsValueChanged::OnValueChanged, object);
}
