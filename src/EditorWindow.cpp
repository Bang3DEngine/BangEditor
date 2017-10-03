#include "BangEditor/EditorWindow.h"

#include "Bang/GL.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Texture2D.h"
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
}

void EditorWindow::Render()
{
    EditorScene *edScene = GetEditorScene(); ENSURE(edScene);
    edScene->RenderAndBlitToScreen();
}

EditorScene *EditorWindow::GetEditorScene() const
{
    return SCAST<EditorScene*>( GetSceneManager()->GetRootScene() );
}
