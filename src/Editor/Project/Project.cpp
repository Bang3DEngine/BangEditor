#include "BangEditor/Project.h"

#include <string>

#include "Bang/Time.h"
#include "Bang/Paths.h"
#include "Bang/Physics.h"
#include "Bang/MetaNode.h"
#include "Bang/Extensions.h"
#include "Bang/SceneManager.h"

#include "BangEditor/SceneOpenerSaver.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Project::Project()
{
    m_id = GUID::GetRandomGUID();
}

Project::~Project()
{
}

Path Project::GetProjectDirectory() const
{
    return GetProjectFilepath().GetDirectory();
}

Path Project::GetProjectAssetsFilepath() const
{
    return Path(GetProjectDirectory().Append("Assets"));
}

Path Project::GetProjectFilepath() const
{
    return m_projectFilepath;
}

String Project::GetProjectName() const
{
    return GetProjectFilepath().GetName();
}

const GUID& Project::GetProjectRandomId() const
{
    return m_id;
}

void Project::SetProjectFilepath(const Path &projectFilepath)
{
    m_projectFilepath = projectFilepath;
}

bool Project::OpenFirstFoundScene() const
{
    Path firstFoundScenePath = GetFirstFoundScenePath();
    if (firstFoundScenePath.IsFile())
    {
        SceneOpenerSaver::GetInstance()->OpenSceneInEditor(firstFoundScenePath);
    }

    return (firstFoundScenePath.IsFile());
}

Path Project::GetFirstFoundScenePath() const
{
    Array<Path> sceneFilepaths = GetProjectAssetsFilepath()
                                .GetFiles(FindFlag::RECURSIVE,
                                           {Extensions::GetSceneExtension()});
    Paths::SortPathsByName(&sceneFilepaths);
    return !sceneFilepaths.IsEmpty() ? sceneFilepaths.Back() : Path::Empty;
}


void Project::ImportMeta(const MetaNode &metaNode)
{
    if (metaNode.Contains("RandomID"))
    {
        m_id = metaNode.Get<GUID>("RandomID");
    }

    if (metaNode.Contains("Physics_StepSleepTime"))
    {
        Physics::GetInstance()->SetStepSleepTime(
                Time::Seconds(metaNode.Get<float>("Physics_StepSleepTime")) );
    }

    if (metaNode.Contains("Physics_MaxSubSteps"))
    {
        Physics::GetInstance()->SetMaxSubSteps(
                    metaNode.Get<int>("Physics_MaxSubSteps"));
    }

    if (metaNode.Contains("Physics_Gravity"))
    {
        Physics::GetInstance()->SetGravity(
                    metaNode.Get<Vector3>("Physics_Gravity"));
    }
}

void Project::ExportMeta(MetaNode *metaNode) const
{
    metaNode->SetName("Project");
    metaNode->Set("RandomID", GetProjectRandomId());
    metaNode->Set("Physics_StepSleepTime",
                 Physics::GetInstance()->GetStepSleepTime().GetSeconds());
    metaNode->Set("Physics_MaxSubSteps",
                 Physics::GetInstance()->GetMaxSubSteps());
    metaNode->Set("Physics_Gravity",
                 Physics::GetInstance()->GetGravity());
}
