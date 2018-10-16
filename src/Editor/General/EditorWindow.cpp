#include "BangEditor/EditorWindow.h"

#include "Bang/String.h"
#include "Bang/Time.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/PlayState.h"
#include "BangEditor/SceneOpenerSaver.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/ShortcutManager.h"

namespace Bang {
class SceneManager;
}  // namespace Bang

USING_NAMESPACE_BANG_EDITOR

EditorWindow::EditorWindow() : Window()
{
    m_shortcutManager = new ShortcutManager();
}

EditorWindow::~EditorWindow()
{
    delete m_shortcutManager;
}

void EditorWindow::Create(uint flags)
{
    Window::Create(flags);
    Maximize();
    SetTitle("Bang Editor - <OpenGL " +
             String(GetGLMajorVersion()) + "." +
             String(GetGLMinorVersion()) + ">");
}

void EditorWindow::Update()
{
    GetShortcutManager()->Update();
    Window::Update();

    float sleepTimeSecs =
            (ScenePlayer::GetPlayState() != PlayState::PLAYING) ? 1.0f : 0.0f;
    SetSleepTimeOnBackground( Time::Seconds(sleepTimeSecs) );
}

void EditorWindow::Render()
{
    Window::Render();
}

bool EditorWindow::OnClosed()
{
    return SceneOpenerSaver::GetInstance()->CloseScene();
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
