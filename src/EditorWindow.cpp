#include "BangEditor/EditorWindow.h"

#include "Bang/Scene.h"
#include "Bang/GEngine.h"
#include "Bang/SceneManager.h"
#include "Bang/UILayoutManager.h"
#include "BangEditor/EditorScene.h"

USING_NAMESPACE_BANG_EDITOR

EditorWindow::EditorWindow()
{

}

EditorWindow::~EditorWindow()
{

}

void EditorWindow::Update()
{
    EditorScene *edScene = GetEditorScene();
    edScene->Update();
    if (edScene->GetOpenScene())
    {
        edScene->GetOpenScene()->Update();
    }
}

void EditorWindow::Render()
{
    Window::Clear();

    EditorScene *edScene = GetEditorScene(); ENSURE(edScene);
    edScene->RenderOpenScene();

    UILayoutManager::RebuildLayout(edScene);
    UILayoutManager::RebuildLayout(edScene->GetOpenScene());
    Window::Render();
}

EditorScene *EditorWindow::GetEditorScene() const
{
    return SCAST<EditorScene*>( GetSceneManager()->GetRootScene() );
}
