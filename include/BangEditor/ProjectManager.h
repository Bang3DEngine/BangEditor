#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "Bang/Bang.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class Project;

class ProjectManager
{
public:
    ProjectManager();

    static Project* OpenProject(const Path &projectFilepath);

    static Project* CreateNewProject(const Path &projectDirPath,
                                     const String &projectName);
    static Project* CreateNewProjectFileOnly(const Path &projectFilepath);
    static void ExportProject(const Project *project);
    static void ExportCurrentProject();
    static void CloseCurrentProject();

    static Project* GetCurrentProject();

private:
    static Project *s_currentProject;
};

NAMESPACE_BANG_EDITOR_END

#endif // PROJECTMANAGER_H
