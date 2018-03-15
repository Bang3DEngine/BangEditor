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
    Debug_Log("Picking as EditorPaths Bang Editor Root: '" << GetEditorDir() << "'");
}

List<Path> EditorPaths::GetEditorIncludeDirs()
{
    List<Path> incDirs;
    incDirs.PushBack( EditorPaths::GetEditorDir().Append("include") );
    return incDirs;
}

Path EditorPaths::GetEditorBinaryDir(BinType binaryType)
{
    String binTypeStr = ( binaryType == BinType::Debug ? "Debug" : "Release" );
    return GetEditorDir().Append("build" + binTypeStr).Append("bin");
}

Path EditorPaths::GetGameExecutableOutputFile(BinType binaryType)
{
    return EditorPaths::GetEditorBinaryDir(binaryType).Append("Game");
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
