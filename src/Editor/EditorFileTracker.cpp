#include "BangEditor/EditorFileTracker.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Shader.h"
#include "Bang/Resources.h"
#include "Bang/ShaderProgram.h"
#include "Bang/CodePreprocessor.h"
#include "Bang/IEventsFileTracker.h"
#include "Bang/MetaFilesManager.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorResources.h"
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
            EventEmitter<IEventsFileTracker>::RegisterListener(this);
    ProjectManager::GetInstance()->
           EventEmitter<IEventsProjectManager>::RegisterListener(this);
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
    MetaFilesManager::OnFilepathRenamed(previousPath, newPath);
}

EditorFileTracker *EditorFileTracker::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetEditorFileTracker();
}

void EditorFileTracker::OnPathAdded(const Path &addedPath)
{
    if (!MetaFilesManager::IsMetaFile(addedPath) &&
        !MetaFilesManager::HasMetaFile(addedPath))
    {
        MetaFilesManager::CreateMetaFileIfMissing(addedPath);
    }
}

void EditorFileTracker::CheckForShaderIncludePathsModifications(const Path &modifiedPath)
{
    // Treat shaderProgram dependencies
    Array<ShaderProgram*> shaderPrograms = Resources::GetAll<ShaderProgram>();
    for (ShaderProgram *sp : shaderPrograms)
    {
        List<Path> shadersIncPaths;
        shadersIncPaths.PushBack(Paths::GetEngineAssetsDir().Append("Shaders"));
        shadersIncPaths.PushBack(EditorPaths::GetEngineAssetsDir().Append("Shaders").
                                Append("Include"));
        shadersIncPaths.PushBack(EditorPaths::GetEditorAssetsDir().Append("Shaders"));
        shadersIncPaths.PushBack(EditorPaths::GetEditorAssetsDir().Append("Shaders").
                                Append("Include"));

        std::array<Shader*, 3> shaders = {{ sp->GetVertexShader(),
                                            sp->GetGeometryShader(),
                                            sp->GetFragmentShader() }};
        for (Shader *shader : shaders)
        {
            if (shader)
            {
                Set<Path> processedPaths;
                List<Path> incPaths = CodePreprocessor::GetSourceIncludePaths(
                                 shader->GetResourceFilepath(), shadersIncPaths);
                for (const Path &incPath : incPaths)
                {
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

void EditorFileTracker::OnPathModified(const Path &modifiedPath)
{
    if (MetaFilesManager::IsMetaFile(modifiedPath)) { return; }

    if (modifiedPath.IsFile())
    {
        CheckForShaderIncludePathsModifications(modifiedPath);

        // Refresh/reimport resources of the modified path
        Resource* modifiedResource = Resources::GetCached(modifiedPath);
        if (modifiedResource)
        {
            Resources::Import(modifiedResource);
        }
    }
}

void EditorFileTracker::OnPathRemoved(const Path &removedPath)
{
    Path importPath = MetaFilesManager::GetMetaFilepath(removedPath);
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

