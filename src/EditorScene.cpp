#include "BangEditor/EditorScene.h"

#include "Bang/UIMask.h"
#include "Bang/Camera.h"
#include "Bang/GEngine.h"
#include "Bang/SubScene.h"
#include "Bang/UICanvas.h"
#include "Bang/Texture2D.h"
#include "Bang/UIBorderRect.h"
#include "Bang/UIGameObject.h"
#include "BangEditor/MenuBar.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorScene::EditorScene()
{
    AddComponent<RectTransform>();
    AddComponent<UICanvas>();

    Camera *cam = AddComponent<Camera>();
    cam->SetClearColor(Color::DarkGray);
    SetCamera(cam);

    m_menuBar = new MenuBar();
    m_menuBar->SetParent(this);

    m_openSceneContainer = GameObjectFactory::CreateUIGameObject();
    RectTransform *openSceneRT = m_openSceneContainer->GetRectTransform();
    openSceneRT->SetMargins(100);
    // m_openSceneImage = m_openSceneContainer->AddComponent<UIImageRenderer>();
    m_openSceneContainer->SetParent(this);
}

EditorScene::~EditorScene()
{
}

#include "Bang/UIFrameLayout.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
void EditorScene::SetOpenScene(Scene *openScene)
{
    /*
    if (p_openScene) { p_openScene->RemoveComponent<SubScene>(); }

    p_openScene = openScene;
    if (p_openScene)
    {
        p_openScene->AddComponent<SubScene>();
        p_openScene->SetFirstFoundCameraOrDefaultOne();
        p_openScene->SetParent(m_openSceneContainer);
    }
    */
    p_openScene = new Scene();
    p_openScene->AddComponent<Transform>();
    p_openScene->AddComponent<SubScene>();
    p_openScene->AddComponent<Camera>()->SetClearColor(Color::Green);
    p_openScene->SetFirstFoundCameraOrDefaultOne();

    UIGameObject *frameLayout = GameObjectFactory::CreateUIGameObject();

    RectTransform *frameLayoutRT = frameLayout->GetComponent<RectTransform>();
    UIFrameLayout *fl = frameLayout->AddComponent<UIFrameLayout>();
    frameLayoutRT->SetMarginTop(-300);
    frameLayoutRT->SetMarginRight(-300);
    frameLayoutRT->SetAnchors( Vector2(-1) );
    fl->SetPaddings(20);
    fl->SetChildrenVerticalStretch( Stretch::Full );
    fl->SetChildrenHorizontalStretch( Stretch::None );
    fl->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    fl->SetChildrenVerticalAlignment(VerticalAlignment::Center);

    UIGameObject *image = GameObjectFactory::CreateUIGameObject();
    image->SetName("I");
    // RectTransform *imageRT = image->GetComponent<RectTransform>();
    UIImageRenderer *ir = image->AddComponent<UIImageRenderer>();
    ir->SetTint(Color::Red);

    UILayoutElement *le = image->AddComponent<UILayoutElement>();
    le->SetMinSize(Vector2i(100,100));
    le->SetPreferredSize(Vector2i(150,150));

    image->SetParent(frameLayout);
    frameLayout->SetParent(p_openScene);


    /*
    UIGameObject *bg = GameObjectFactory::CreateUIGameObject();
    bg->AddComponent<UIImageRenderer>()->SetTint(Color::Red);
    bg->SetParent(p_openScene);

    UIGameObject *menu = GameObjectFactory::CreateUIGameObject();
    UIDirLayout *dirLayout = menu->AddComponent<UIHorizontalLayout>();
    dirLayout->SetSpacing(15);
    menu->SetParent(bg);

    UIGameObject *textCont1 = GameObjectFactory::CreateUIGameObject();
    textCont1->AddComponent<UIVerticalLayout>();
    textCont1->SetParent(menu);

    UIGameObject *textGo1 = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *text1 = textGo1->AddComponent<UITextRenderer>();
    text1->SetContent("HOLA");
    textGo1->SetParent(textCont1);
    UIGameObject *textGo1_2 = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *text1_2 = textGo1_2->AddComponent<UITextRenderer>();
    text1_2->SetContent("HEHEHEHEHE");
    textGo1_2->SetParent(textCont1);

    UIGameObject *textGo2 = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *text2 = textGo2->AddComponent<UITextRenderer>();
    text2->SetContent("JOLINES");
    textGo2->SetParent(menu);

    UIGameObject *textGo3 = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *text3 = textGo3->AddComponent<UITextRenderer>();
    text3->SetContent("ADIOS");
    textGo3->SetParent(menu);
    */

    p_openScene->SetParent(m_openSceneContainer);
}

Scene *EditorScene::GetOpenScene() const
{
    return p_openScene;
}
