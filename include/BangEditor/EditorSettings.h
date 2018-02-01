#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

#include "Bang/Path.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorSettings
{
public:

    static void SetLatestProjectFilepathOpen(const Path &latestProjectFilePathOpen);
    static Path GetLatestProjectFilepathOpen();

private:
    Path m_latestProjectFileOpen = Path::Empty;

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

