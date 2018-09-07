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

    if (EditorPaths::GetEditorAssetsDir().IsDir())
    {
        Debug_Log("Picking as EditorPaths Bang Editor Root: '" <<
                  EditorPaths::GetEditorDir() << "'");
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
    return EditorPaths::GetEditorDir().Append("Assets");
}

Path EditorPaths::GetEditorBinariesDir()
{
    return EditorPaths::GetEditorDir().
           Append("Binaries").
           Append( Paths::GetBuildType() );
}

Path EditorPaths::GetEditorLibrariesDir()
{
    return EditorPaths::GetEditorDir().
           Append("Libraries").
           Append(Paths::GetBuildType());
}

Path EditorPaths::GetEditorBuildDir()
{
    return EditorPaths::GetEditorDir().Append("Build");
}

Path EditorPaths::GetBangStaticLibPath()
{
    return EditorPaths::GetEditorLibrariesDir().Append("libBang.a");
}

Path EditorPaths::GetBangDynamicLibPath()
{
    return EditorPaths::GetEditorLibrariesDir().Append("libBang.so");
}

Path EditorPaths::CreateEditorPath(const String &path)
{
    return EditorPaths::GetEditorAssetsDir().Append(path);
}

EditorPaths *EditorPaths::GetInstance()
{
    return EditorApplication::GetInstance()->GetEditorPaths();
}

Bang::Path BangEditor::EditorPaths::GetBangLatestLibPath()
{
    Path staticLibPath = EditorPaths::GetBangStaticLibPath();
    Path dynamicLibPath = EditorPaths::GetBangDynamicLibPath();
    if (staticLibPath.IsFile() &&
        (staticLibPath.GetModificationTimeSeconds() >
         dynamicLibPath.GetModificationTimeSeconds())
       )
    {
        return staticLibPath;
    }
    return dynamicLibPath;
}
