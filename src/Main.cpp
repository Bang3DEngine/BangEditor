#include <ostream>

#include "Bang/Application.h"
#include "Bang/BangDefines.h"
#include "Bang/Debug.h"
#include "Bang/Path.h"
#include "Bang/Paths.h"
#include "Bang/StreamOperators.h"
#include "Bang/Window.h"
#include "Bang/WindowManager.h"
#include "Bang/WindowManager.tcc"
#include "BangEditor/BangEditor.h"
#include "BangEditor/EditorApplication.h"
#include "BangEditor/EditorProjectManager.h"
#include "BangEditor/EditorWindow.h"
#include "BangEditor/SelectProjectWindow.h"

using namespace Bang;
using namespace BangEditor;

int main(int argc, char **argv)
{
    EditorApplication editorApplication;
    const Path edtPath = Paths::GetResolvedPath(Paths::GetExecutablePath()
                                                    .GetDirectory()
                                                    .GetDirectory()
                                                    .GetDirectory());
    const Path engPath = Paths::GetResolvedPath(edtPath.Append("Bang"));
    Debug_Log("BangEngineRoot: " << engPath);
    Debug_Log("BangEditorRoot: " << edtPath);
    editorApplication.Init(engPath, edtPath);

    Path projectToBeOpen = Path::Empty();
    if (argc < 2)
    {
        // Select project window
        SelectProjectWindow *selectProjectWindow =
            WindowManager::CreateWindow<SelectProjectWindow>();
        Window::SetActive(selectProjectWindow);
        selectProjectWindow->Init();
        editorApplication.MainLoop();

        projectToBeOpen = SelectProjectWindow::SelectedProjectPath;
    }
    else
    {
        projectToBeOpen = Path(argv[1]);
    }

    if (projectToBeOpen.IsEmpty())
    {
        Debug_Log("No project selected.");
        Application::Exit(0, true);
    }

    if (!projectToBeOpen.IsFile())
    {
        Debug_Error("Could not open project '" << projectToBeOpen << "'");
        Application::Exit(1, true);
    }

    // Main window
    Window *mainWindow = WindowManager::CreateWindow<EditorWindow>();
    Window::SetActive(mainWindow);
    editorApplication.OpenEditorScene();

    EditorProjectManager::GetInstance()->OpenProject(projectToBeOpen);

    return editorApplication.MainLoop();
}
