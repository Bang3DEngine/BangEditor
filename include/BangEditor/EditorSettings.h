#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

#include "Bang/Array.h"
#include "Bang/BangDefines.h"
#include "Bang/Path.h"
#include "Bang/Settings.h"

#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace BangEditor
{
class EditorSettings : public Settings
{
public:
    static void AddRecentProjectFilepathOpen(
        const Path &recentProjectFilePathOpen);
    static const Array<Path> &GetRecentProjectFilepathsOpen();

private:
    Array<Path> m_recentProjectFilesOpen;

    EditorSettings();
    virtual ~EditorSettings();

    void Init() override;
    void ExportToFile();
    void ImportFromFile();
    static Path GetEditorSettingsPath();

    static EditorSettings *GetInstance();

    friend class Editor;
    friend class EditorScene;
    friend class EditorApplication;
};
}  // namespace BangEditor

#endif  // EDITORSETTINGS_H
