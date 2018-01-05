#include "BangEditor/BehaviourContainer.h"

#include "Bang/XMLNode.h"
#include "Bang/Application.h"

#include "BangEditor/Behaviour.h"
#include "BangEditor/BehaviourManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

BehaviourContainer::BehaviourContainer()
{
}

BehaviourContainer::~BehaviourContainer()
{
}

void BehaviourContainer::SetSourceFilepath(const Path &sourceFilepath)
{
    if (sourceFilepath != GetSourceFilepath())
    {
        m_sourceFilepath = sourceFilepath;
    }
}

Behaviour *BehaviourContainer::CreateBehaviourInstance() const
{
    return BehaviourManager::CreateBehaviourInstance(GetBehaviourName());
}

String BehaviourContainer::GetBehaviourName() const
{
    return GetSourceFilepath().GetName();
}

const Path &BehaviourContainer::GetSourceFilepath() const
{
    return m_sourceFilepath;
}
bool BehaviourContainer::IsLoaded() const
{
    return BehaviourManager::IsCompiled( GetSourceFilepath() );
}

void BehaviourContainer::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    BehaviourContainer *bc = Cast<BehaviourContainer*>(clone);
    bc->SetSourceFilepath( GetSourceFilepath() );
}

void BehaviourContainer::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("SourceFilepath"))
    { SetSourceFilepath( xmlInfo.Get<Path>("SourceFilepath") ); }
}

void BehaviourContainer::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("SourceFilepath", GetSourceFilepath());
}
