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
    m_modifiedPathsFromLastUpdate.Clear();
    m_fileTracker.Update();
}

bool BehaviourTracker::HasBeenModified(const Path &sourcePath) const
{
    List<Path> includePaths =
        CodePreprocessor::GetSourceIncludePaths(sourcePath, GetIncludePaths());
    for (const Path &includePath : includePaths)
    {
        if (HasBeenModified(includePath)) { return true; }
    }
    return m_modifiedPathsFromLastUpdate.Contains(sourcePath);
}

List<Path> BehaviourTracker::GetIncludePaths() const
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
    m_fileTracker.RemovePath( EditorPaths::GetProjectAssetsDir() );
}

void BehaviourTracker::OnProjectOpen(const Project *project)
{
    m_fileTracker.AddPath( EditorPaths::GetProjectAssetsDir() );
}

void BehaviourTracker::OnPathAdded(const Path &addedPath)
{
    m_modifiedPathsFromLastUpdate.Add(addedPath);
}

void BehaviourTracker::OnPathModified(const Path &modifiedPath)
{
    m_modifiedPathsFromLastUpdate.Add(modifiedPath);
}

void BehaviourTracker::OnPathRemoved(const Path &removedPath)
{
    m_modifiedPathsFromLastUpdate.Add(removedPath);
}

