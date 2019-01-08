#ifndef SELECTPROJECTWINDOW_H
#define SELECTPROJECTWINDOW_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsFocus.h"
#include "Bang/Path.h"
#include "Bang/Scene.h"
#include "Bang/String.h"
#include "Bang/UIFocusable.h"
#include "Bang/Window.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class UIButton;
}

using namespace Bang;
namespace BangEditor
{
class SelectProjectWindow : public Window
{
public:
    static Path SelectedProjectPath;

    SelectProjectWindow();
    virtual ~SelectProjectWindow() override;

    void Init();
};

// ==========================================================================

class SelectProjectScene : public Scene
{
public:
    SelectProjectScene();
    virtual ~SelectProjectScene() override;

private:
    class RecentProjectListEntry : public GameObject
    {
        GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(RecentProjectListEntry);

    public:
        RecentProjectListEntry();
        RecentProjectListEntry(const Path &projectPath);
        virtual ~RecentProjectListEntry() override;

        Path m_projectPath = Path::Empty();
    };

    Path m_selectedRecentPath = Path::Empty();
    UIButton *p_newProjectButton = nullptr;
    UIButton *p_openProjectButton = nullptr;
    UIButton *p_openSelectedProjectButton = nullptr;

    // GameObject
    void Update() override;

    void NewProject();
    void OpenProject();
    void ConfirmOpenProject(const Path &projectFilepath);
};
}  // namespace BangEditor

#endif  // SELECTPROJECTWINDOW_H
