#include "BangEditor/BehaviourTracker.h"

#include "Bang/Paths.h"
#include "Bang/CodePreprocessor.h"

#include "BangEditor/EditorPaths.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

BehaviourTracker::BehaviourTracker()
{
    ProjectManager::GetInstance()->RegisterListener(this);
    m_fileTracker.EventEmitter<IFileTrackerListener>::RegisterListener(this);

    m_fileTracker.SetCheckFrequencySeconds(2.0);
}

BehaviourTracker::~BehaviourTracker()
{
}

void BehaviourTracker::Update()
{
    m_changedPathsFromLastUpdate.Clear();
    m_fileTracker.Update();
}

void BehaviourTracker::CheckForChanges()
{
    m_fileTracker.CheckForChanges();
}

bool BehaviourTracker::HasBeenModified(const Path &sourcePath) const
{
    List<Path> includePaths =
        CodePreprocessor::GetSourceIncludePaths(sourcePath, GetIncludeDirs(),
                                                true);
    for (const Path &includePath : includePaths)
    {
        if (HasBeenModified(includePath)) { return true; }
    }
    return m_changedPathsFromLastUpdate.Contains(sourcePath);
}

List<Path> BehaviourTracker::GetIncludeDirs() const
{
    List<Path> includePaths;
    includePaths.PushBack( EditorPaths::GetProjectIncludeDirs() );
    return includePaths;
}

const FileTracker &BehaviourTracker::GetFileTracker() const
{
    return m_fileTracker;
}

void BehaviourTracker::OnProjectClosed(const Project *project)
{
    m_fileTracker.UnTrackPath( EditorPaths::GetProjectAssetsDir() );
}

void BehaviourTracker::OnProjectOpen(const Project *project)
{
    m_fileTracker.TrackPath( EditorPaths::GetProjectAssetsDir() );
}

void BehaviourTracker::OnPathAdded(const Path &addedPath)
{
    m_changedPathsFromLastUpdate.Add(addedPath);
}

void BehaviourTracker::OnPathModified(const Path &modifiedPath)
{
    m_changedPathsFromLastUpdate.Add(modifiedPath);
}

void BehaviourTracker::OnPathRemoved(const Path &removedPath)
{
    m_changedPathsFromLastUpdate.Add(removedPath);
}

