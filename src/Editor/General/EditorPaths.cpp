#include "BangEditor/EditorPaths.h"

#include <ostream>

#include "Bang/Application.h"
#include "Bang/Debug.h"
#include "Bang/Extensions.h"
#include "Bang/List.tcc"
#include "Bang/StreamOperators.h"
#include "Bang/Time.h"
#include "BangEditor/EditorApplication.h"

using namespace Bang;
using namespace BangEditor;

EditorPaths::EditorPaths()
{
}

EditorPaths::~EditorPaths()
{
}

void EditorPaths::InitEditorPath(const Path &editorRootPath)
{
    m_editorRootPath = editorRootPath;

    if (EditorPaths::GetEditorAssetsDir().IsDir())
    {
        Debug_DLog("Picking as EditorPaths Bang Editor Root: '"
                   << EditorPaths::GetEditorDir() << "'");
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
    incDirs.PushBack(EditorPaths::GetEditorDir().Append("include"));
    return incDirs;
}

const Path &EditorPaths::GetEditorDir()
{
    return EditorPaths::GetInstance()->m_editorRootPath;
}

Path EditorPaths::GetEditorAssetsDir()
{
    return EditorPaths::GetEditorDir().Append("Assets");
}

Path EditorPaths::GetEditorBinariesDir()
{
    return EditorPaths::GetEditorDir()
        .Append("Binaries")
        .Append(Paths::GetBuildType());
}

Path EditorPaths::GetEditorLibrariesDir()
{
    return EditorPaths::GetEditorDir()
        .Append("Libraries")
        .Append(Paths::GetBuildType());
}

Path EditorPaths::GetEditorBuildDir()
{
    return EditorPaths::GetEditorDir().Append("Build");
}

Path EditorPaths::GetBangStaticLibPath()
{
    return EditorPaths::GetEditorLibrariesDir().Append("libBang").WithExtension(
        Extensions::GetStaticLibExtension());
}

Path EditorPaths::GetBangDynamicLibPath()
{
    return EditorPaths::GetEditorLibrariesDir().Append("libBang").WithExtension(
        Extensions::GetDynamicLibExtension());
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
    if (staticLibPath.IsFile() && (staticLibPath.GetModificationTime() >
                                   dynamicLibPath.GetModificationTime()))
    {
        return staticLibPath;
    }
    return dynamicLibPath;
}
