#include "BangEditor/SceneTab.h"

#include "Bang/Rect.h"
#include "Bang/UIButton.h"
#include "Bang/Texture2D.h"
#include "Bang/IconManager.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneContainer.h"
#include "BangEditor/EditorIconManager.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

SceneTab::SceneTab()
{
    SetName("SceneTab");

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2(6.0f, 1.0f) );

    GameObjectFactory::CreateUIGameObjectInto(this);

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetChildrenHorizontalStretch(Stretch::Full);
    vl->SetPaddings(10);
    vl->SetSpacing(3);

    GameObject *toolbarGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *toolbarHL = toolbarGo->AddComponent<UIHorizontalLayout>();
    toolbarHL->SetSpacing(6);
    toolbarGo->SetParent(this);

    constexpr int ToolBarHeight = 15;

    UILayoutElement *toolbarLE = toolbarGo->AddComponent<UILayoutElement>();
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
    p_playButton->GetGameObject()->SetParent(toolbarGo);

    p_stopButton = GameObjectFactory::CreateUIButton();
    p_stopButton->SetIcon(EditorIconManager::GetSquareIcon().Get(),
                          Vector2i(10));
    p_stopButton->GetIcon()->SetTint(Color::DarkGray);
    p_stopButton->GetLayoutElement()->SetMinSize( Vector2i(ToolBarHeight) );
    p_stopButton->GetFocusable()->AddClickedCallback([this](IFocusable*)
    {
        ScenePlayer::StopScene();
    });
    p_stopButton->GetGameObject()->SetParent(toolbarGo);

    GameObject *stateTextGo = GameObjectFactory::CreateUIGameObject();
    p_stateText = stateTextGo->AddComponent<UITextRenderer>();
    p_stateText->SetTextSize(10);
    stateTextGo->SetParent(toolbarGo);

    p_sceneContainer = GameObject::Create<UISceneContainer>();
    p_sceneContainer->SetParent(this);

    Editor::RegisterListener<IEditorSelectionListener>(this);
}

SceneTab::~SceneTab()
{
}

void SceneTab::OnPlayScene()
{
    p_stateText->SetContent("Playing scene");
    p_playButton->SetBlocked(true);
    p_stopButton->SetBlocked(false);
}

void SceneTab::OnStopScene()
{
    p_stateText->SetContent("Editing scene");
    p_playButton->SetBlocked(false);
    p_stopButton->SetBlocked(true);
}

void SceneTab::SetSceneImageTexture(Texture2D *sceneTexture)
{
    p_sceneContainer->SetSceneImageTexture(sceneTexture);
}

Rect SceneTab::GetSceneImageRectNDC() const
{
    return p_sceneContainer->GetRectTransform()->GetViewportRectNDC();
}

void SceneTab::OnPlayStateChanged(EditorPlayState previousPlayState,
                                  EditorPlayState newPlayState)
{
    switch (newPlayState)
    {
        case EditorPlayState::Editing: OnStopScene(); break;
        case EditorPlayState::Playing: OnPlayScene(); break;
    }
}

