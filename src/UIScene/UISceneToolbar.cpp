#include "BangEditor/UISceneToolbar.h"

#include "Bang/UIButton.h"
#include "Bang/UICheckBox.h"
#include "Bang/IconManager.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/ScenePlayer.h"
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

    p_stopButton = GameObjectFactory::CreateUIButton();
    p_stopButton->SetIcon(EditorIconManager::GetSquareIcon().Get(),
                          Vector2i(10));
    p_stopButton->GetIcon()->SetTint(Color::DarkGray);
    p_stopButton->GetLayoutElement()->SetMinSize( Vector2i(ToolBarHeight) );
    p_stopButton->GetFocusable()->AddClickedCallback([this](IFocusable*)
    {
        ScenePlayer::StopScene();
    });

    p_showDebugStatsCheckbox = GameObjectFactory::CreateUICheckBox();
    p_showDebugStatsCheckbox->SetChecked(false);
    p_showDebugStatsCheckbox->EventEmitter<IValueChangedListener>::RegisterListener(this);

    GameObject *showDebugStatsTextGo = GameObjectFactory::CreateUIGameObject();
    UITextRenderer*showDebugStatsText =
                        showDebugStatsTextGo->AddComponent<UITextRenderer>();
    showDebugStatsText->SetTextSize(10);
    showDebugStatsText->SetContent("Stats");

    p_playButton->GetGameObject()->SetParent(this);
    p_stopButton->GetGameObject()->SetParent(this);
    GameObjectFactory::CreateUISpacer(LayoutSizeType::Flexible, Vector2::One)->
                        SetParent(this);
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

void UISceneToolbar::OnPlayScene()
{
    p_playButton->SetBlocked(true);
    p_stopButton->SetBlocked(false);
}

void UISceneToolbar::OnStopScene()
{
    p_playButton->SetBlocked(false);
    p_stopButton->SetBlocked(true);
}


void UISceneToolbar::OnPlayStateChanged(EditorPlayState previousPlayState,
                                        EditorPlayState newPlayState)
{
    switch (newPlayState)
    {
        case EditorPlayState::Editing: OnStopScene(); break;
        case EditorPlayState::Playing: OnPlayScene(); break;
    }
}

void UISceneToolbar::OnValueChanged(Object *object)
{
    EventEmitter<IValueChangedListener>::PropagateToListeners(
                    &IValueChangedListener::OnValueChanged, object);
}
