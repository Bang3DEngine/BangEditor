#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "Bang/Bang.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/IEventsProjectManager.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ProjectManager : public EventEmitter<IEventsProjectManager>
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
