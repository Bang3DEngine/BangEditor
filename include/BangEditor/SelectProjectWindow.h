#ifndef SELECTPROJECTWINDOW_H
#define SELECTPROJECTWINDOW_H

#include "Bang/Bang.h"
#include "Bang/Path.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/UIFocusable.h"
#include "Bang/IEventsFocus.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class SelectProjectWindow : public Window
{
public:
    static Path SelectedProjectPath;

    SelectProjectWindow();
    virtual ~SelectProjectWindow();

    void Init();
};

// ==========================================================================

class SelectProjectScene : public Scene
{
public:
    SelectProjectScene();
    virtual ~SelectProjectScene();

private:
    class RecentProjectListEntry : public GameObject
    {
        GAMEOBJECT(RecentProjectListEntry);

    public:
        RecentProjectListEntry();
        RecentProjectListEntry(const Path &projectPath);
        virtual ~RecentProjectListEntry();

        Path m_projectPath = Path::Empty;
    };

    Path m_selectedRecentPath = Path::Empty;
    UIButton *p_newProjectButton          = nullptr;
    UIButton *p_openProjectButton         = nullptr;
    UIButton *p_openSelectedProjectButton = nullptr;

    // GameObject
    void Update() override;

    void NewProject();
    void OpenProject();
    void ConfirmOpenProject(const Path &projectFilepath);
};


NAMESPACE_BANG_EDITOR_END

#endif // SELECTPROJECTWINDOW_H

