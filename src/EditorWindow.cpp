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
#include "BangEditor/ShortcutManager.h"
#include "BangEditor/SceneOpenerSaver.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG_EDITOR

EditorWindow::EditorWindow()
{
    m_shortcutManager = new ShortcutManager();
}

EditorWindow::~EditorWindow()
{
    delete m_shortcutManager;
}

void EditorWindow::Update()
{
    GetShortcutManager()->Update();
    GetSceneManager()->Update();
}

void EditorWindow::Render()
{
    EditorScene *edScene = EditorSceneManager::GetEditorScene();
    if (edScene)
    {
        edScene->RenderAndBlitToScreen();
    }
}

void EditorWindow::OnClosed()
{
    SceneOpenerSaver::GetInstance()->CloseScene();
}

void EditorWindow::Create(uint flags)
{
    Window::Create(flags);
    Maximize();
}

SceneManager *EditorWindow::CreateSceneManager() const
{
    return new EditorSceneManager();
}

ShortcutManager *EditorWindow::GetShortcutManager() const
{
    return m_shortcutManager;
}

EditorWindow *EditorWindow::GetActive()
{
    return DCAST<EditorWindow*>( Window::GetActive() );
}
