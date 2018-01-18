#include "BangEditor/UISceneToolbar.h"

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
#include "BangEditor/EditorIconManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UISceneToolbar::UISceneToolbar()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *toolbarHL = AddComponent<UIHorizontalLayout>();
    toolbarHL->SetSpacing(6);

    constexpr int ToolBarHeight = 15;

    UILayoutElement *toolbarLE = AddComponent<UILayoutElement>();
    toolbarLE->SetPreferredHeight(ToolBarHeight);
    toolbarLE->SetFlexibleWidth( 1.0f );

    p_playButton = GameObjectFactory::CreateUIButton();
    p_playButton->SetIcon(IconManager::GetRightArrowIcon().Get(),
                          Vector2i(10));
    p_playButton->GetLayoutElement()->SetMinSize( Vector2i(ToolBarHeight) );
    p_playButton->GetIcon()->SetTint(Color::DarkGray);
    p_playButton->GetFocusable()->AddClickedCallback([this](IFocusable*)
    {
        ScenePlayer::PlayScene();
    });

    p_pauseButton = GameObjectFactory::CreateUIButton();
    p_pauseButton->SetIcon(EditorIconManager::GetDoubleBarIcon().Get(),
                           Vector2i(10));
    p_pauseButton->GetLayoutElement()->SetMinSize( Vector2i(ToolBarHeight) );
    p_pauseButton->GetIcon()->SetTint(Color::DarkGray);
    p_pauseButton->GetFocusable()->AddClickedCallback([this](IFocusable*)
    {
        ScenePlayer::PauseScene();
    });

    p_stepButton = GameObjectFactory::CreateUIButton();
    p_stepButton->SetIcon(EditorIconManager::GetRightArrowAndBarIcon().Get(),
                          Vector2i(10));
    p_stepButton->GetLayoutElement()->SetMinSize( Vector2i(ToolBarHeight) );
    p_stepButton->GetIcon()->SetTint(Color::DarkGray);
    p_stepButton->GetFocusable()->AddClickedCallback([this](IFocusable*)
    {
        ScenePlayer::StepFrame();
    });

    p_stopButton = GameObjectFactory::CreateUIButton();
    p_stopButton->SetIcon(EditorIconManager::GetSquareIcon().Get(),
                          Vector2i(10));
    p_stopButton->GetIcon()->SetTint(Color::DarkGray);
    p_stopButton->GetLayoutElement()->SetMinSize( Vector2i(ToolBarHeight) );
    p_stopButton->GetFocusable()->AddClickedCallback([this](IFocusable*)
    {
        ScenePlayer::StopScene();
    });

    p_renderModeInput = GameObjectFactory::CreateUIComboBox();
    p_renderModeInput->AddItem("Color",     SCAST<int>(UISceneImage::RenderMode::Color));
    p_renderModeInput->AddItem("Diffuse",   SCAST<int>(UISceneImage::RenderMode::Diffuse));
    p_renderModeInput->AddItem("Normal",    SCAST<int>(UISceneImage::RenderMode::Normal));
    // p_renderModeInput->AddItem("Depth",     SCAST<int>(UISceneImage::RenderMode::Depth));
    p_renderModeInput->AddItem("Selection", SCAST<int>(UISceneImage::RenderMode::Selection));
    p_renderModeInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_showDebugStatsCheckbox = GameObjectFactory::CreateUICheckBox();
    p_showDebugStatsCheckbox->SetChecked(false);
    p_showDebugStatsCheckbox->EventEmitter<IValueChangedListener>::RegisterListener(this);

    GameObject *showDebugStatsTextGo = GameObjectFactory::CreateUIGameObject();
    UITextRenderer*showDebugStatsText =
                        showDebugStatsTextGo->AddComponent<UITextRenderer>();
    showDebugStatsText->SetTextSize(10);
    showDebugStatsText->SetContent("Stats");

    p_playButton->GetGameObject()->SetParent(this);
    p_pauseButton->GetGameObject()->SetParent(this);
    p_stepButton->GetGameObject()->SetParent(this);
    p_stopButton->GetGameObject()->SetParent(this);
    GameObjectFactory::CreateUISpacer(LayoutSizeType::Flexible, Vector2::One)->
                        SetParent(this);
    p_renderModeInput->GetGameObject()->SetParent(this);
    showDebugStatsTextGo->SetParent(this);
    p_showDebugStatsCheckbox->GetGameObject()->SetParent(this);

    Editor::GetInstance()->EventEmitter<IEditorListener>::RegisterListener(this);
}

UISceneToolbar::~UISceneToolbar()
{
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
    p_stopButton->SetBlocked(true);
}

void UISceneToolbar::OnStepFrameScene()
{
    OnPauseScene();
}

void UISceneToolbar::OnStopScene()
{
    p_playButton->SetBlocked(false);
    p_pauseButton->SetBlocked(true);
    p_stepButton->SetBlocked(true);
    p_stopButton->SetBlocked(true);
}

void UISceneToolbar::OnPlayStateChanged(EditorPlayState,
                                        EditorPlayState newPlayState)
{
    switch (newPlayState)
    {
        case EditorPlayState::Editing:   OnStopScene();      break;
        case EditorPlayState::Paused:    OnPauseScene();     break;
        case EditorPlayState::StepFrame: OnStepFrameScene(); break;
        case EditorPlayState::Playing:   OnPlayScene();      break;
    }
}

void UISceneToolbar::OnValueChanged(Object *object)
{
    EventEmitter<IValueChangedListener>::PropagateToListeners(
                    &IValueChangedListener::OnValueChanged, object);
}
