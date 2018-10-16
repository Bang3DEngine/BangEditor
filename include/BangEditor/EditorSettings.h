#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

#include "Bang/Array.h"
#include "Bang/BangDefines.h"
#include "Bang/Path.h"
#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorSettings
{
public:

    static void AddRecentProjectFilepathOpen(const Path &recentProjectFilePathOpen);
    static const Array<Path>& GetRecentProjectFilepathsOpen();

private:
    Array<Path> m_recentProjectFilesOpen;

    EditorSettings();
    virtual ~EditorSettings();

    void Init();
    void ExportToFile();
    void ImportFromFile();
    static Path GetEditorSettingsPath();

    static EditorSettings* GetInstance();

    friend class Editor;
    friend class EditorScene;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORSETTINGS_H

