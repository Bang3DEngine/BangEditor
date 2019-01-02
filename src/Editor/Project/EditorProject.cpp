#include "BangEditor/EditorProject.h"

#include "Bang/MetaFilesManager.h"
#include "BangEditor/SceneOpenerSaver.h"

using namespace Bang;
using namespace BangEditor;

EditorProject::EditorProject()
{
}

EditorProject::~EditorProject()
{
}

void EditorProject::SetLastOpenScenePath(const Path &lastOpenScenePath)
{
    if (lastOpenScenePath != GetLastOpenScenePath())
    {
        m_lastOpenScenePath = lastOpenScenePath;
        ExportMetaToFile(GetProjectFilepath());
    }
}

const Path &EditorProject::GetLastOpenScenePath() const
{
    return m_lastOpenScenePath;
}

bool EditorProject::OpenInitialScene() const
{
    Path scenePathToOpen = Path::Empty();
    {
        if (GetLastOpenScenePath().IsFile())
        {
            scenePathToOpen = GetLastOpenScenePath();
        }
        else
        {
            scenePathToOpen = GetInitialScenePath();
        }
    }

    if (scenePathToOpen.IsFile())
    {
        SceneOpenerSaver::GetInstance()->OpenSceneInEditor(scenePathToOpen);
        return true;
    }
    return false;
}

void EditorProject::ImportMeta(const MetaNode &metaNode)
{
    Project::ImportMeta(metaNode);

    if (metaNode.Contains("LastOpenSceneGUID"))
    {
        GUID lastOpenSceneGUID = metaNode.Get<GUID>("LastOpenSceneGUID");
        Path lastOpenScenePath =
            MetaFilesManager::GetFilepath(lastOpenSceneGUID);
        SetLastOpenScenePath(lastOpenScenePath);
    }
}

void EditorProject::ExportMeta(MetaNode *metaNode) const
{
    Project::ExportMeta(metaNode);

    GUID lastOpenSceneGUID = MetaFilesManager::GetGUID(GetLastOpenScenePath());
    metaNode->Set("LastOpenSceneGUID", lastOpenSceneGUID);
}
