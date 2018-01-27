#ifndef EDITORPATHS_H
#define EDITORPATHS_H

#include "Bang/Paths.h"

#include "BangEditor/BangEditor.h"

#define EDPATH(path) BangEditor::EditorPaths::CreateEditorPath(path)  // Engine assets path
#define PPATH(path)  BangEditor::EditorPaths::CreateProjectPath(path) // Project assets path

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorPaths : public Paths
{
public:
    void InitEditorPath(const Path &editorRootPath);

    static const Path& GetProjectDir();
    static Path GetProjectAssetsDir();
    static Path GetProjectLibrariesDir();

    static const Path& GetEditorDir();
    static Path GetEditorResourcesDir();

    static List<Path> GetAllProjectSubDirs();
    static List<Path> GetEditorIncludeDirs();
    static List<Path> GetProjectIncludeDirs();
    static Path GetEditorBinaryDir(BinType binaryType);
    static Path GetGameExecutableOutputFile(BinType binaryType);

    static Path CreateEditorPath(const String &path);
    static Path CreateProjectPath(const String &path);

    static void SetProjectRoot(const Path &projectRootDir);

private:
    Path c_editorRoot  = Path::Empty;
    Path c_projectRoot = Path::Empty;

    EditorPaths();
    virtual ~EditorPaths();

    static EditorPaths* GetInstance();

    friend class EditorApplication;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORPATHS_H

