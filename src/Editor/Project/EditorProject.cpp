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
    Path firstFoundScenePath = GetFirstFoundScenePath();
    if (firstFoundScenePath.IsFile())
    {
        SceneOpenerSaver::GetInstance()->OpenSceneInEditor(firstFoundScenePath);
    }
    return firstFoundScenePath.IsFile();
}
