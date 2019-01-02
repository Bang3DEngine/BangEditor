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
    void SetRecentProjectFilepathsOpen(
        const Array<Path> &recentProjectFilepathsOpen);
    void AddRecentProjectFilepathOpen(const Path &recentProjectFilepathOpen);
    const Array<Path> &GetRecentProjectFilepathsOpen();

    static EditorSettings *GetInstance();

private:
    Array<Path> m_recentProjectFilesOpen;

    EditorSettings();
    virtual ~EditorSettings() override;

    void Init() override;
    void ExportToFile();
    void ImportFromFile();
    static Path GetEditorSettingsPath();

    friend class Editor;
    friend class EditorScene;
    friend class EditorApplication;
    friend class EditorProjectManager;
};
}  // namespace BangEditor

#endif  // EDITORSETTINGS_H
