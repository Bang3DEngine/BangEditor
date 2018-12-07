#include "BangEditor/EditorMeshFactoryUITab.h"

#include "Bang/GameObjectFactory.h"
#include "Bang/UIButton.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "BangEditor/EditorMeshFactoryUITabScene.h"
#include "BangEditor/EditorTextureFactory.h"

using namespace Bang;
using namespace BangEditor;

EditorMeshFactoryUITab::EditorMeshFactoryUITab()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    AddComponent<UIVerticalLayout>();
    UILayoutElement *mainLE = AddComponent<UILayoutElement>();
    mainLE->SetFlexibleSize(Vector2::One());

    GameObject *toolbar = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *toolbarHL = toolbar->AddComponent<UIHorizontalLayout>();
    toolbarHL->SetPaddings(5, 5, 5, 5);

    p_centerSceneButton = GameObjectFactory::CreateUIButton(
        "", EditorTextureFactory::GetEyeIcon());
    p_centerSceneButton->GetIcon()->SetTint(Color::DarkGray());
    p_centerSceneButton->AddClickedCallback(
        [this]() { p_meshFactoryUITabScene->ResetCamera(); });
    p_centerSceneButton->GetGameObject()->SetParent(toolbar);

    GameObject *botHLGo = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *botHLLE = botHLGo->AddComponent<UILayoutElement>();
    botHLLE->SetFlexibleSize(Vector2::One());
    botHLLE->SetMinWidth(300);
    botHLLE->SetPreferredWidth(600);

    UIHorizontalLayout *botHL = botHLGo->AddComponent<UIHorizontalLayout>();
    botHL->SetChildrenVerticalStretch(Stretch::FULL);
    botHL->SetPaddings(20);

    AddComponent<UIFocusable>();

    p_sidebar = GameObjectFactory::CreateUIGameObject();
    {
        UILayoutElement *sidebarLE = p_sidebar->AddComponent<UILayoutElement>();
        sidebarLE->SetFlexibleSize(Vector2(0.5f, 1.0f));
        sidebarLE->SetMinWidth(200);

        UIVerticalLayout *inspVL = p_sidebar->AddComponent<UIVerticalLayout>();
        inspVL->SetPaddingLeft(10);
    }

    toolbar->SetParent(this);
    GameObjectFactory::CreateUIHSeparator(LayoutSizeType::MIN, 10)
        ->SetParent(this);

    botHLGo->SetParent(this);

    p_meshFactoryUITabScene = new EditorMeshFactoryUITabScene();
    p_meshFactoryUITabScene->SetParent(botHLGo);

    // GameObjectFactory::CreateUIVSeparator()->SetParent(botHLGo);
    p_sidebar->SetParent(botHLGo);
    GameObjectFactory::CreateUIHSeparator(LayoutSizeType::MIN, 20)
        ->SetParent(this);
}

EditorMeshFactoryUITab::~EditorMeshFactoryUITab()
{
}
