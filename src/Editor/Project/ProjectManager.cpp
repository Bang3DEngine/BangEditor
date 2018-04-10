#include "BangEditor/ProjectManager.h"

#include "Bang/Path.h"
#include "Bang/File.h"
#include "Bang/Debug.h"
#include "Bang/String.h"
#include "Bang/XMLNode.h"
#include "Bang/Extensions.h"
#include "Bang/XMLNodeReader.h"
#include "Bang/ImportFilesManager.h"

#include "BangEditor/Project.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSettings.h"
#include "BangEditor/SceneOpenerSaver.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Project *ProjectManager::s_currentProject = nullptr;

ProjectManager::ProjectManager()
{
}

Project* ProjectManager::OpenProject(const Path &projectFilepath)
{
    if (!ProjectManager::CloseCurrentProject()) { return nullptr; }

    ProjectManager::s_currentProject = new Project();
    Project *currentProject = ProjectManager::s_currentProject;
    currentProject->ImportXMLFromFile(projectFilepath);
    currentProject->SetProjectFilepath( projectFilepath );

    Paths::SetProjectRoot(currentProject->GetProjectDirectory());

    Path assetsDir = currentProject->GetProjectAssetsFilepath();
    ImportFilesManager::CreateMissingImportFiles(assetsDir);
    ImportFilesManager::LoadImportFilepathGUIDs(assetsDir);

    ProjectManager::GetInstance()->
        EventEmitter<IProjectManagerListener>::PropagateToListeners(
               &IProjectManagerListener::OnProjectOpen, s_currentProject);

    EditorSettings::SetLatestProjectFilepathOpen(
                            currentProject->GetProjectFilepath() );

    bool sceneOpen = currentProject->OpenFirstFoundScene();
    if (!sceneOpen) { SceneOpenerSaver::GetInstance()->OnNewScene(); }

    return currentProject;
}

Project* ProjectManager::CreateNewProject(const Path &projectDirPath,
                                          const String &projectName)
{
    if (!ProjectManager::CloseCurrentProject()) { return nullptr; }

    Path projectDir(projectDirPath + "/" + projectName);
    if (!projectDir.Exists())
    {
        if (!File::CreateDirectory(projectDir))
        {
            Debug_Error ("Could not create project in directory '" <<
                         projectDir << "'.");
            return nullptr;
        }
    }
    else
    {
        // TODO: Add overwrite window warning
        Debug_Warn("Directory '" << projectDir << "' already existed, using it.");
    }

    Path projectFilepath(projectDir + "/" + projectName + "." +
                             Extensions::GetProjectExtension());

    ProjectManager::s_currentProject = CreateNewProjectFileOnly(projectFilepath);
    ProjectManager::s_currentProject->SetProjectFilepath(projectFilepath);

    File::CreateDirectory(projectDir.Append("Assets"));
    File::CreateDirectory(projectDir.Append("Libraries"));

    return ProjectManager::s_currentProject;
}

Project *ProjectManager::CreateNewProjectFileOnly(const Path &projectFilepath)
{
    Project *proj = new Project();
    proj->ExportXMLToFile(projectFilepath);
    return proj;
}

void ProjectManager::ExportProject(const Project *project)
{
    if (!project) { return; }
    bool ok = project->ExportXMLToFile(project->GetProjectFilepath());
    if (ok)
    {
        Debug_Log("Project '" << project->GetProjectName() <<
                  "' successfully saved.");
    }
    else
    {
        Debug_Error("Could not save the project...");
    }
}

void ProjectManager::ExportCurrentProject()
{
    ProjectManager::ExportProject( ProjectManager::GetCurrentProject() );
}

bool ProjectManager::CloseCurrentProject()
{
    if (!SceneOpenerSaver::GetInstance()->CloseScene()) { return false; }

    if (ProjectManager::s_currentProject)
    {
        ProjectManager::GetInstance()->
            EventEmitter<IProjectManagerListener>::PropagateToListeners(
                   &IProjectManagerListener::OnProjectClosed, s_currentProject);

        delete s_currentProject;
        ProjectManager::s_currentProject = nullptr;
    }
    return true;
}

Project *ProjectManager::GetCurrentProject()
{
    return ProjectManager::s_currentProject;
}

ProjectManager *ProjectManager::GetInstance()
{
    EditorScene *edScene = EditorSceneManager::GetEditorScene();
    return edScene ? edScene->GetProjectManager() : nullptr;
}
