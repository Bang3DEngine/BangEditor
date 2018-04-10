#include "BangEditor/BehaviourTracker.h"

#include "Bang/Debug.h"
#include "Bang/Paths.h"
#include "Bang/CodePreprocessor.h"

#include "BangEditor/EditorPaths.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

BehaviourTracker::BehaviourTracker()
{
    m_fileTracker.EventEmitter<IFileTrackerListener>::RegisterListener(this);
    m_fileTracker.SetCheckFrequencySeconds(4.0);
}

BehaviourTracker::~BehaviourTracker()
{
}

void BehaviourTracker::Update(bool forceCheckNow)
{
    m_fileTracker.Update(forceCheckNow);

    // Handle project changing
    if (m_previousProjectPath != Paths::GetProjectAssetsDir())
    {
        m_fileTracker.UnTrackPath(m_previousProjectPath);

        m_previousProjectPath = Paths::GetProjectAssetsDir();
        m_fileTracker.TrackPath( Paths::GetProjectAssetsDir() );
    }
}

void BehaviourTracker::ResetModifications()
{
    m_changedPathsFromLastUpdate.Clear();
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
    includePaths.PushBack( Paths::GetProjectIncludeDirs() );
    return includePaths;
}

const FileTracker &BehaviourTracker::GetFileTracker() const
{
    return m_fileTracker;
}

void BehaviourTracker::OnPathAdded(const Path &addedPath)
{
    m_changedPathsFromLastUpdate.Add(addedPath);
}

void BehaviourTracker::OnPathModified(const Path &modifiedPath)
{
    // Debug_DLog("Behaviour modified: " << modifiedPath);
    m_changedPathsFromLastUpdate.Add(modifiedPath);
}

void BehaviourTracker::OnPathRemoved(const Path &removedPath)
{
    m_changedPathsFromLastUpdate.Add(removedPath);
}

