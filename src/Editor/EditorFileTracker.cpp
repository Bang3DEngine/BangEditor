#include "BangEditor/EditorFileTracker.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Shader.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/ShaderProgram.h"
#include "Bang/CodePreprocessor.h"
#include "Bang/IEventsFileTracker.h"
#include "Bang/MetaFilesManager.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorResources.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorFileTracker::EditorFileTracker()
{
    m_fileTracker = new FileTracker();

    GetFileTracker()->TrackPath( Paths::GetEngineAssetsDir() );
    GetFileTracker()->TrackPath( Paths::GetProjectAssetsDir() );
    GetFileTracker()->TrackPath( Paths::GetProjectLibrariesDir() );
    GetFileTracker()->TrackPath( EditorPaths::GetEditorAssetsDir() );

    GetFileTracker()->
            EventEmitter<IEventsFileTracker>::RegisterListener(this);
    ProjectManager::GetInstance()->
           EventEmitter<IEventsProjectManager>::RegisterListener(this);
}

EditorFileTracker::~EditorFileTracker()
{
    delete m_fileTracker;
}

void EditorFileTracker::CheckFiles()
{
    GetFileTracker()->CheckFiles();
}

FileTracker* EditorFileTracker::GetFileTracker() const
{
    return m_fileTracker;
}

void EditorFileTracker::OnPathRenamed(const Path &previousPath, const Path &newPath)
{
    MetaFilesManager::OnFilepathRenamed(previousPath, newPath);
}

const USet<Path> &EditorFileTracker::GetTrackedPaths() const
{
    return GetFileTracker()->GetTrackedPaths();
}

Array<Path> EditorFileTracker::GetTrackedPathsWithExtensions(
                                    const Array<String> &extensions) const
{
    return GetFileTracker()->GetTrackedPathsWithExtensions(extensions);
}

Array<Path> EditorFileTracker::GetTrackedPathsWithLastExtension(
                                    const Array<String> &extensions) const
{
    return GetFileTracker()->GetTrackedPathsWithLastExtension(extensions);
}

EditorFileTracker *EditorFileTracker::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetEditorFileTracker();
}

void EditorFileTracker::CheckForShaderModifications(const Path &modifiedPath)
{
    if (!Extensions::Equals(modifiedPath.GetExtension(),
                            Extensions::GetShaderExtensions()))
    {
        return;
    }

    Array<Path> shadersIncPaths;
    shadersIncPaths.PushBack(Paths::GetEngineAssetsDir().Append("Shaders"));
    shadersIncPaths.PushBack(EditorPaths::GetEngineAssetsDir().Append("Shaders").
                            Append("Include"));
    shadersIncPaths.PushBack(EditorPaths::GetEditorAssetsDir().Append("Shaders"));
    shadersIncPaths.PushBack(EditorPaths::GetEditorAssetsDir().Append("Shaders").
                            Append("Include"));

    // Treat shaderProgram dependencies
    Array<ShaderProgram*> shaderPrograms = Resources::GetAll<ShaderProgram>();
    for (ShaderProgram *sp : shaderPrograms)
    {
        std::array<Shader*, 3> shaders = {{ sp->GetVertexShader(),
                                            sp->GetGeometryShader(),
                                            sp->GetFragmentShader() }};
        for (Shader *shader : shaders)
        {
            if (shader)
            {
                Set<Path> processedPaths;
                Array<Path> incPaths = CodePreprocessor::GetSourceIncludePaths(
                                                   shader->GetResourceFilepath(),
                                                   shadersIncPaths);
                for (int i = 0; i < incPaths.Size(); ++i) // const Path &incPath : incPaths)
                {
                    const Path incPath = incPaths[i];
                    if (incPath == modifiedPath)
                    {
                        Resources::Import(shader);
                        break;
                    }
                    else if (!processedPaths.Contains(incPath))
                    {
                        incPaths.PushBack(
                             CodePreprocessor::GetSourceIncludePaths(
                                        incPath, shadersIncPaths)); // Recursive
                        processedPaths.Add(incPath);
                    }
                }
            }
        }
    }
}

void EditorFileTracker::CheckForBehaviourModifications(const Path &modifiedPath)
{

}

void EditorFileTracker::OnPathAdded(const Path &addedPath)
{
    if (!MetaFilesManager::IsMetaFile(addedPath) &&
        !MetaFilesManager::HasMetaFile(addedPath))
    {
        MetaFilesManager::CreateMetaFileIfMissing(addedPath);
    }

    EventEmitter<IEventsFileTracker>::PropagateToListeners(
                &IEventsFileTracker::OnPathAdded, addedPath);
}

void EditorFileTracker::OnPathModified(const Path &modifiedPath)
{
    if (!MetaFilesManager::IsMetaFile(modifiedPath))
    {
        if (modifiedPath.IsFile())
        {
            CheckForShaderModifications(modifiedPath);
            CheckForBehaviourModifications(modifiedPath);

            // Refresh/reimport resources of the modified path
            Resource* modifiedResource = Resources::GetCached(modifiedPath);
            if (modifiedResource)
            {
                Resources::Import(modifiedResource);
            }
        }
    }

    EventEmitter<IEventsFileTracker>::PropagateToListeners(
                &IEventsFileTracker::OnPathModified, modifiedPath);
}

void EditorFileTracker::OnPathRemoved(const Path &removedPath)
{
    Path importPath = MetaFilesManager::GetMetaFilepath(removedPath);
    File::Remove(importPath);

    EventEmitter<IEventsFileTracker>::PropagateToListeners(
                &IEventsFileTracker::OnPathRemoved, removedPath);
}

void EditorFileTracker::OnProjectOpen(const Project*)
{
    GetFileTracker()->TrackPath( Paths::GetProjectAssetsDir() );
    GetFileTracker()->TrackPath( Paths::GetProjectLibrariesDir() );
}

void EditorFileTracker::OnProjectClosed(const Project*)
{
    GetFileTracker()->UnTrackPath( Paths::GetProjectAssetsDir() );
    GetFileTracker()->UnTrackPath( Paths::GetProjectLibrariesDir() );
}

