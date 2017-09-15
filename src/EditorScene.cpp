#include "BangEditor/EditorScene.h"

#include "Bang/UIMask.h"
#include "Bang/Camera.h"
#include "Bang/GEngine.h"
#include "Bang/Texture2D.h"
#include "Bang/UIBorderRect.h"
#include "Bang/UIGameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/SubSceneContainer.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorScene::EditorScene()
{
    AddComponent<RectTransform>();

    Camera *cam = AddComponent<Camera>();
    cam->SetClearColor(Color::Gray);
    SetCamera(cam);

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
    if (p_openScene) { p_openScene->RemoveComponent<SubSceneContainer>(); }

    p_openScene = openScene;
    if (p_openScene)
    {
        p_openScene->AddComponent<SubSceneContainer>();
        p_openScene->SetFirstFoundCameraOrDefaultOne();
        p_openScene->SetParent(m_openSceneContainer);
    }
}

Scene *EditorScene::GetOpenScene() const
{
    return p_openScene;
}
