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

namespace Bang
{
class Path;
}

using namespace Bang;
namespace BangEditor
{
class IEventsProjectManager;
class Project;

class ProjectManager : public EventEmitter<IEventsProjectManager>
{
public:
    ProjectManager();

    static Project *OpenProject(const Path &projectFilepath);

    static Project *CreateNewProject(const Path &projectDirPath,
                                     const String &projectName);
    static Project *CreateNewProjectFileOnly(const Path &projectFilepath);
    static void ExportProject(const Project *project);
    static void ExportCurrentProject();
    static bool CloseCurrentProject();

    static Project *GetCurrentProject();

    static ProjectManager *GetInstance();

private:
    static Project *s_currentProject;
};
}

#endif  // PROJECTMANAGER_H
