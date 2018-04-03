#ifndef EDITORPATHS_H
#define EDITORPATHS_H

#include "Bang/Paths.h"

#include "BangEditor/BangEditor.h"

#define EDPATH(path) BangEditor::EditorPaths::CreateEditorPath(path)  // Engine assets path

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorPaths : public Paths
{
public:
    void InitEditorPath(const Path &editorRootPath);

    static const Path& GetEditorDir();
    static Path GetEditorAssetsDir();

    static List<Path> GetEditorIncludeDirs();

    static Path CreateEditorPath(const String &path);

private:
    Path c_editorRoot  = Path::Empty;

    EditorPaths();
    virtual ~EditorPaths();

    static EditorPaths* GetInstance();

    friend class EditorApplication;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORPATHS_H

