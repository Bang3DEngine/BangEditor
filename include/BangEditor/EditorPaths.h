#ifndef EDITORPATHS_H
#define EDITORPATHS_H

#include "Bang/Paths.h"

#include "BangEditor/BangEditor.h"

#define EDPATH(path) Paths::MakeEditorPath(path)  // Engine assets path
#define PPATH(path)  Paths::MakeProjectPath(path) // Project assets path

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorPaths : public Paths
{
public:
    void InitEditorPath(const Path &editorRootPath);

    static const Path& Project();
    static Path ProjectAssets();
    static Path ProjectLibrariesDir();

    static const Path& Editor();
    static Path EditorResources();

    static List<Path> GetAllProjectSubDirs();
    static List<Path> GetEditorIncludeDirs();
    static List<Path> GetProjectIncludeDirs();
    static Path EditorBinaryDir(BinType binaryType);
    static Path GameExecutableOutputFile(BinType binaryType);

    static Path MakeEditorPath(const String &path);
    static Path MakeProjectPath(const String &path);

    static void SetEditorRoot(const Path &editorRootDir);
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

