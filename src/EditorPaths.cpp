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
    List<Path> subdirs = EditorPaths::Project()
                         .FindSubDirectories(Path::FindFlag::Recursive);
    subdirs.PushFront(EditorPaths::Project());
    return subdirs;
}

List<Path> EditorPaths::GetEditorIncludeDirs()
{
    List<Path> incDirs;
    incDirs.PushBack( EditorPaths::Editor().Append("include") );
    return incDirs;
}

List<Path> EditorPaths::GetProjectIncludeDirs()
{
    List<Path> incDirs = EditorPaths::ProjectAssets()
                        .FindSubDirectories(Path::FindFlag::Recursive);
    incDirs.PushBack( EditorPaths::ProjectAssets() );
    return incDirs;
}

Path EditorPaths::EditorBinaryDir(BinType binaryType)
{
    String binTypeStr = ( binaryType == BinType::Debug ? "Debug" : "Release" );
    return Editor().Append("build" + binTypeStr).Append("bin");
}

Path EditorPaths::GameExecutableOutputFile(BinType binaryType)
{
    return EditorPaths::EditorBinaryDir(binaryType).Append("Game");
}

const Path &EditorPaths::Project()
{
    return EditorPaths::GetInstance()->c_projectRoot;
}

Path EditorPaths::ProjectAssets()
{
    return Project().Append("Assets");
}

Path EditorPaths::ProjectLibrariesDir()
{
    return Project().Append("Libraries");
}

const Path &EditorPaths::Editor()
{
    return EditorPaths::GetInstance()->c_editorRoot;
}

Path EditorPaths::EditorResources()
{
    return Editor().Append("res");
}


Path EditorPaths::MakeEditorPath(const String &path)
{
    return EditorPaths::EditorResources().Append(path);
}

Path EditorPaths::MakeProjectPath(const String &path)
{
    return EditorPaths::ProjectAssets().Append(path);
}

void EditorPaths::SetProjectRoot(const Path &projectRootDir)
{
    EditorPaths::GetInstance()->c_projectRoot = projectRootDir;
}

EditorPaths *EditorPaths::GetInstance()
{
    return EditorApplication::GetInstance()->GetEditorPaths();
}
