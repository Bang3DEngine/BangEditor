#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class Project;

class IProjectManagerListener : public IEventListener
{
public:
    virtual void OnProjectClosed(const Project *project) { (void)project; }
    virtual void OnProjectOpen(const Project *project) { (void)project; }
};

class ProjectManager : public EventEmitter<IProjectManagerListener>
{
public:
    ProjectManager();

    static Project* OpenProject(const Path &projectFilepath);

    static Project* CreateNewProject(const Path &projectDirPath,
                                     const String &projectName);
    static Project* CreateNewProjectFileOnly(const Path &projectFilepath);
    static void ExportProject(const Project *project);
    static void ExportCurrentProject();
    static bool CloseCurrentProject();

    static Project* GetCurrentProject();

    static ProjectManager *GetInstance();

private:
    static Project *s_currentProject;
};

NAMESPACE_BANG_EDITOR_END

#endif // PROJECTMANAGER_H
