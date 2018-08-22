#include "BangEditor/Project.h"

#include <string>

#include "Bang/Time.h"
#include "Bang/Paths.h"
#include "Bang/XMLNode.h"
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
    List<Path> sceneFilepaths = GetProjectAssetsFilepath()
                                .GetFiles(Path::FindFlag::RECURSIVE,
                                           {Extensions::GetSceneExtension()});
    Paths::SortPathsByName(&sceneFilepaths);
    return !sceneFilepaths.IsEmpty() ? sceneFilepaths.Back() : Path::Empty;
}


void Project::ImportXML(const XMLNode &xmlInfo)
{
    if (xmlInfo.Contains("RandomID"))
    {
        m_id = xmlInfo.Get<GUID>("RandomID");
    }
}

void Project::ExportXML(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("Project");
    xmlInfo->Set("RandomID", m_id);
}
