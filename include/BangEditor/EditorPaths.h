#ifndef EDITORPATHS_H
#define EDITORPATHS_H

#include "Bang/BangDefines.h"
#include "Bang/List.h"
#include "Bang/Path.h"
#include "Bang/Paths.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

#define EDPATH(path) \
    BangEditor::EditorPaths::CreateEditorPath(path)  // Engine assets path

using namespace Bang;
namespace BangEditor
{
class EditorPaths : public Paths
{
public:
    void InitEditorPath(const Path &editorRootPath);

    static const Path &GetEditorDir();
    static Path GetEditorAssetsDir();
    static Path GetEditorBinariesDir();
    static Path GetEditorLibrariesDir();
    static Path GetEditorBuildDir();
    static Path GetBangStaticLibPath();
    static Path GetBangDynamicLibPath();
    static Path GetBangLatestLibPath();

    static List<Path> GetEditorIncludeDirs();

    static Path CreateEditorPath(const String &path);

private:
    Path c_editorRoot = Path::Empty;

    EditorPaths();
    virtual ~EditorPaths() override;

    static EditorPaths *GetInstance();

    friend class EditorApplication;
};
}

#endif  // EDITORPATHS_H
