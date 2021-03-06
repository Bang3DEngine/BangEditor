#include "BangEditor/EditorFileTracker.h"

#include <array>
#include <functional>

#include "Bang/Assets.h"
#include "Bang/Assets.tcc"
#include "Bang/CodePreprocessor.h"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/File.h"
#include "Bang/FileTracker.h"
#include "Bang/IEventsFileTracker.h"
#include "Bang/IEventsProjectManager.h"
#include "Bang/MetaFilesManager.h"
#include "Bang/Path.h"
#include "Bang/Paths.h"
#include "Bang/Set.h"
#include "Bang/Set.tcc"
#include "Bang/Shader.h"
#include "Bang/ShaderProgram.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorProjectManager.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"

namespace Bang
{
class Asset;
class String;
class Project;
}

using namespace Bang;
using namespace BangEditor;

EditorFileTracker::EditorFileTracker()
{
    m_fileTracker = new FileTracker();

    GetFileTracker()->TrackPath(Paths::GetEngineAssetsDir());
    GetFileTracker()->TrackPath(Paths::GetProjectAssetsDir());
    GetFileTracker()->TrackPath(Paths::GetProjectLibrariesDir());
    GetFileTracker()->TrackPath(EditorPaths::GetEditorAssetsDir());

    GetFileTracker()->EventEmitter<IEventsFileTracker>::RegisterListener(this);
    EditorProjectManager::GetInstance()
        ->EventEmitter<IEventsProjectManager>::RegisterListener(this);
}

EditorFileTracker::~EditorFileTracker()
{
    delete m_fileTracker;
}

void EditorFileTracker::CheckFiles()
{
    GetFileTracker()->CheckFiles();
}

FileTracker *EditorFileTracker::GetFileTracker() const
{
    return m_fileTracker;
}

void EditorFileTracker::OnPathRenamed(const Path &previousPath,
                                      const Path &newPath)
{
    MetaFilesManager::OnFilepathRenamed(previousPath, newPath);
}

Time EditorFileTracker::GetModificationTime(const Path &path) const
{
    return m_fileTracker->GetModificationTime(path);
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
    if (EditorScene *edScene = EditorSceneManager::GetEditorScene())
    {
        return edScene->GetEditorFileTracker();
    }
    return nullptr;
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
    shadersIncPaths.PushBack(
        EditorPaths::GetEngineAssetsDir().Append("Shaders").Append("Include"));
    shadersIncPaths.PushBack(
        EditorPaths::GetEditorAssetsDir().Append("Shaders"));
    shadersIncPaths.PushBack(
        EditorPaths::GetEditorAssetsDir().Append("Shaders").Append("Include"));

    // Treat shaderProgram dependencies
    Array<ShaderProgram *> shaderPrograms = Assets::GetAll<ShaderProgram>();
    for (ShaderProgram *sp : shaderPrograms)
    {
        std::array<Shader *, 3> shaders = {{sp->GetVertexShader(),
                                            sp->GetGeometryShader(),
                                            sp->GetFragmentShader()}};
        for (Shader *shader : shaders)
        {
            if (shader)
            {
                Set<Path> processedPaths;
                Array<Path> incPaths = CodePreprocessor::GetSourceIncludePaths(
                    shader->GetAssetFilepath(), shadersIncPaths);
                for (uint i = 0; i < incPaths.Size();
                     ++i)  // const Path &incPath : incPaths)
                {
                    const Path incPath = incPaths[i];
                    if (incPath == modifiedPath)
                    {
                        Assets::Import(shader);
                        break;
                    }
                    else if (!processedPaths.Contains(incPath))
                    {
                        incPaths.PushBack(
                            CodePreprocessor::GetSourceIncludePaths(
                                incPath, shadersIncPaths));  // Recursive
                        processedPaths.Add(incPath);
                    }
                }
            }
        }
    }
}

void EditorFileTracker::CheckForBehaviourModifications(const Path &)
{
}

void EditorFileTracker::OnPathAdded(const Path &addedPath)
{
    MetaFilesManager::RegisterFilepath(addedPath);

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

            // Refresh/reimport assets of the modified path
            Asset *modifiedAsset = Assets::GetCached(modifiedPath);
            if (modifiedAsset)
            {
                Assets::Import(modifiedAsset);
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

void EditorFileTracker::OnProjectOpen(const Project *)
{
    GetFileTracker()->TrackPath(Paths::GetProjectAssetsDir());
    GetFileTracker()->TrackPath(Paths::GetProjectLibrariesDir());
}

void EditorFileTracker::OnProjectClosed(const Project *)
{
    GetFileTracker()->UnTrackPath(Paths::GetProjectAssetsDir());
    GetFileTracker()->UnTrackPath(Paths::GetProjectLibrariesDir());
}
