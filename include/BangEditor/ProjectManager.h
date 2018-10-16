#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/IEventsProjectManager.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Path;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class IEventsProjectManager;
FORWARD class Project;

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
