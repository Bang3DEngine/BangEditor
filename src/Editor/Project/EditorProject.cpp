#include "BangEditor/EditorProject.h"

#include "BangEditor/SceneOpenerSaver.h"

using namespace Bang;
using namespace BangEditor;

EditorProject::EditorProject()
{
}

EditorProject::~EditorProject()
{
}

bool EditorProject::OpenInitialScene() const
{
    Path initialScenePath = GetInitialScenePath();
    if (initialScenePath.IsFile())
    {
        SceneOpenerSaver::GetInstance()->OpenSceneInEditor(initialScenePath);
    }
    return initialScenePath.IsFile();
}
