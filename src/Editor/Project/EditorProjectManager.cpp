#include "BangEditor/EditorProjectManager.h"

#include "Bang/Extensions.h"
#include "Bang/File.h"
#include "Bang/IEventsProjectManager.h"
#include "Bang/MetaFilesManager.h"
#include "Bang/Paths.h"
#include "Bang/Settings.h"
#include "BangEditor/EditorProject.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorSettings.h"
#include "BangEditor/QtProjectManager.h"
#include "BangEditor/SceneOpenerSaver.h"

using namespace Bang;
using namespace BangEditor;

EditorProjectManager::EditorProjectManager()
{
}

EditorProjectManager::~EditorProjectManager()
{
}

Project *EditorProjectManager::OpenProject(const Path &projectFilepath)
{
    Project *openProject = ProjectManager::OpenProject(projectFilepath);
    if (openProject)
    {
        EditorSettings::GetInstance()->AddRecentProjectFilepathOpen(
            openProject->GetProjectFilepath());

        bool sceneHasBeenOpen = openProject->OpenInitialScene();
        SceneOpenerSaver *sov = SceneOpenerSaver::GetInstance();
        if (!sceneHasBeenOpen && sov)
        {
            sov->OnNewScene();
        }

        QtProjectManager::CreateQtProjectFile();
    }
    return openProject;
}
EditorProject *EditorProjectManager::CreateNewProject(
    const Path &projectDirPath,
    const String &projectName)
{
    if (!EditorProjectManager::CloseCurrentProject())
    {
        return nullptr;
    }

    Path projectDir(projectDirPath.Append(projectName));
    if (!projectDir.Exists())
    {
        if (!File::CreateDir(projectDir))
        {
            Debug_Error("Could not create project in directory '" << projectDir
                                                                  << "'.");
            return nullptr;
        }
    }
    else
    {
        // TODO: Add overwrite window warning
        Debug_Warn("Directory '" << projectDir
                                 << "' already existed, using it.");
    }

    Path projectFilepath(projectDir.Append(projectName)
                             .WithExtension(Extensions::GetProjectExtension()));

    SetCurrentProject(CreateNewProjectFileOnly(projectFilepath));
    GetCurrentProject()->SetProjectFilepath(projectFilepath);

    File::CreateDir(projectDir.Append("Assets"));
    File::CreateDir(projectDir.Append("Libraries"));

    return GetCurrentProject();
}

EditorProject *EditorProjectManager::CreateNewProjectFileOnly(
    const Path &projectFilepath)
{
    EditorProject *proj = new EditorProject();
    proj->ExportMetaToFile(projectFilepath);
    return proj;
}

void EditorProjectManager::ExportProject(const EditorProject *project)
{
    if (!project)
    {
        return;
    }
    bool ok = project->ExportMetaToFile(project->GetProjectFilepath());
    if (!ok)
    {
        Debug_Error("Could not save the project...");
    }
}

void EditorProjectManager::ExportCurrentProject()
{
    ExportProject(GetCurrentProject());
}

bool EditorProjectManager::CloseCurrentProject()
{
    if (!SceneOpenerSaver::GetInstance())
    {
        return true;
    }

    if (!SceneOpenerSaver::GetInstance()->CloseScene())
    {
        return false;
    }

    if (GetCurrentProject())
    {
        EventEmitter<IEventsProjectManager>::PropagateToListeners(
            &IEventsProjectManager::OnProjectClosed, GetCurrentProject());
    }

    return ProjectManager::CloseCurrentProject();
}

EditorProject *EditorProjectManager::GetCurrentProject() const
{
    return SCAST<EditorProject *>(ProjectManager::GetCurrentProject());
}

EditorProjectManager *EditorProjectManager::GetInstance()
{
    return SCAST<EditorProjectManager *>(ProjectManager::GetInstance());
}

Project *EditorProjectManager::CreateNewProject() const
{
    return new EditorProject();
}
