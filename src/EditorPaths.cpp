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

List<Path> EditorPaths::GetProjectIncludeDirs()
{
    List<Path> subdirs = EditorPaths::ProjectAssets()
                        .FindSubDirectories(Path::FindFlag::Recursive);
    return subdirs;
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
