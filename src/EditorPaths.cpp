#include "BangEditor/EditorPaths.h"

#include "Bang/Debug.h"
#include "BangEditor/EditorApplication.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorPaths::EditorPaths()
{
}

EditorPaths::~EditorPaths()
{
}

void EditorPaths::InitEditorPath(const Path &editorRootPath)
{
    c_editorRoot = editorRootPath;

    if (GetEditorAssetsDir().IsDir())
    {
        Debug_Log("Picking as EditorPaths Bang Editor Root: '" << GetEditorDir() << "'");
    }
    else
    {
        Debug_Error("Could not find an Editor directory!");
        Application::Exit(1, true);
    }
}

List<Path> EditorPaths::GetEditorIncludeDirs()
{
    List<Path> incDirs;
    incDirs.PushBack( EditorPaths::GetEditorDir().Append("include") );
    return incDirs;
}

const Path &EditorPaths::GetEditorDir()
{
    return EditorPaths::GetInstance()->c_editorRoot;
}

Path EditorPaths::GetEditorAssetsDir()
{
    return GetEditorDir().Append("Assets");
}

Path EditorPaths::CreateEditorPath(const String &path)
{
    return EditorPaths::GetEditorAssetsDir().Append(path);
}

EditorPaths *EditorPaths::GetInstance()
{
    return EditorApplication::GetInstance()->GetEditorPaths();
}
