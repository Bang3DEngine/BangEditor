#include "BangEditor/EditorFileTracker.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Resources.h"
#include "Bang/ImportFilesManager.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/BehaviourTracker.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorFileTracker::EditorFileTracker()
{
    m_fileTracker = new FileTracker();
    m_behaviourFileTracker = new BehaviourTracker();

    GetFileTracker()->SetCheckFrequencySeconds(3.0f);
    GetFileTracker()->TrackPath( Paths::GetEngineAssetsDir() );
    GetFileTracker()->TrackPath( Paths::GetProjectAssetsDir() );
    GetFileTracker()->TrackPath( EditorPaths::GetEditorAssetsDir() );

    GetFileTracker()->
            EventEmitter<IFileTrackerListener>::RegisterListener(this);
    ProjectManager::GetInstance()->
           EventEmitter<IProjectManagerListener>::RegisterListener(this);
}

EditorFileTracker::~EditorFileTracker()
{
    delete m_fileTracker;
    delete m_behaviourFileTracker;
}

FileTracker* EditorFileTracker::GetFileTracker() const
{
    return m_fileTracker;
}

BehaviourTracker *EditorFileTracker::GetBehaviourTracker() const
{
    return m_behaviourFileTracker;
}

void EditorFileTracker::OnPathRenamed(const Path &previousPath, const Path &newPath)
{
    ImportFilesManager::OnFilepathRenamed(previousPath, newPath);
}

EditorFileTracker *EditorFileTracker::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetEditorFileTracker();
}

void EditorFileTracker::OnPathAdded(const Path &addedPath)
{
    if (!ImportFilesManager::IsImportFile(addedPath) &&
        !ImportFilesManager::HasImportFile(addedPath))
    {
        ImportFilesManager::CreateImportFile(addedPath);
    }
}

void EditorFileTracker::OnPathModified(const Path &modifiedPath)
{
    if (ImportFilesManager::IsImportFile(modifiedPath)) { return; }

    if (modifiedPath.IsFile())
    {
        // Refresh/reimport resources of the modified path
        Array<Resource*> pathResources = Resources::GetCached(modifiedPath);
        for (Resource *pathRes : pathResources)
        {
            Resources::Import(pathRes);
        }
    }
}

void EditorFileTracker::OnPathRemoved(const Path &removedPath)
{
    Path importPath = ImportFilesManager::GetImportFilepath(removedPath);
    File::Remove(importPath);
}

void EditorFileTracker::OnProjectOpen(const Project*)
{
    GetFileTracker()->TrackPath( Paths::GetProjectAssetsDir() );
}

void EditorFileTracker::OnProjectClosed(const Project*)
{
    GetFileTracker()->UnTrackPath( Paths::GetProjectAssetsDir() );
}

