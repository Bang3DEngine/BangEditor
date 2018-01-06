#include "BangEditor/EditorPaths.h"

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
}

List<Path> EditorPaths::GetAllProjectSubDirs()
{
    List<Path> subdirs = EditorPaths::GetProjectDir()
                         .FindSubDirectories(Path::FindFlag::Recursive);
    subdirs.PushFront(EditorPaths::GetProjectDir());
    return subdirs;
}

List<Path> EditorPaths::GetEditorIncludeDirs()
{
    List<Path> incDirs;
    incDirs.PushBack( EditorPaths::GetEditorDir().Append("include") );
    return incDirs;
}

List<Path> EditorPaths::GetProjectIncludeDirs()
{
    List<Path> incDirs = EditorPaths::GetProjectAssetsDir()
                        .FindSubDirectories(Path::FindFlag::Recursive);
    incDirs.PushBack( EditorPaths::GetProjectAssetsDir() );
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

const Path &EditorPaths::GetProjectDir()
{
    return EditorPaths::GetInstance()->c_projectRoot;
}

Path EditorPaths::GetProjectAssetsDir()
{
    return GetProjectDir().Append("Assets");
}

Path EditorPaths::GetProjectLibrariesDir()
{
    return GetProjectDir().Append("Libraries");
}

const Path &EditorPaths::GetEditorDir()
{
    return EditorPaths::GetInstance()->c_editorRoot;
}

Path EditorPaths::GetEditorResourcesDir()
{
    return GetEditorDir().Append("res");
}

Path EditorPaths::CreateEditorPath(const String &path)
{
    return EditorPaths::GetEditorResourcesDir().Append(path);
}

Path EditorPaths::CreateProjectPath(const String &path)
{
    return EditorPaths::GetProjectAssetsDir().Append(path);
}

void EditorPaths::SetProjectRoot(const Path &projectRootDir)
{
    EditorPaths::GetInstance()->c_projectRoot = projectRootDir;
}

EditorPaths *EditorPaths::GetInstance()
{
    return EditorApplication::GetInstance()->GetEditorPaths();
}
