#include "BangEditor/EditorScene.h"

#include "Bang/UIMask.h"
#include "Bang/Camera.h"
#include "Bang/GEngine.h"
#include "Bang/SubScene.h"
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

    Camera *cam = AddComponent<Camera>();
    cam->SetClearColor(Color::DarkGray);
    SetCamera(cam);

    m_menuBar = new MenuBar();
    m_menuBar->SetParent(this);

    m_openSceneContainer = GameObjectFactory::CreateUIGameObject();
    RectTransform *openSceneRT = m_openSceneContainer->GetRectTransform();
    openSceneRT->SetMargins(100);
    m_openSceneImage = m_openSceneContainer->AddComponent<UIImageRenderer>();
    m_openSceneContainer->SetParent(this);
}

EditorScene::~EditorScene()
{
}

void EditorScene::SetOpenScene(Scene *openScene)
{
    if (p_openScene) { p_openScene->RemoveComponent<SubScene>(); }

    p_openScene = openScene;
    if (p_openScene)
    {
        p_openScene->AddComponent<SubScene>();
        p_openScene->SetFirstFoundCameraOrDefaultOne();
        p_openScene->SetParent(m_openSceneContainer);
    }
}

Scene *EditorScene::GetOpenScene() const
{
    return p_openScene;
}
