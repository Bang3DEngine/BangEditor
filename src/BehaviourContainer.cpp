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

Behaviour *BehaviourContainer::CreateBehaviour(const String &behaviourName,
                                               Library *openLibrary)
{
    Library *lib = openLibrary;
    if (!lib) { return nullptr; }

    if (!lib->IsLoaded()) { lib->Load(); }

    String errorString = "";
    if (lib->IsLoaded())
    {
        // Get the pointer to the CreateDynamically function
        String funcName = "CreateDynamically_" + behaviourName;
        Behaviour* (*createFunction)(Application*) =
            lib->Get< Behaviour*(*)(Application*) >(funcName.ToCString());

        if (createFunction)
        {
            // Call it and get the pointer to the created Behaviour
            // Create the Behaviour, passing to it the Application
            // of this main binary, so it can link it.
            return createFunction(Application::GetInstance());
        }
        else { errorString = lib->GetErrorString(); }
    }
    else { errorString = lib->GetErrorString(); }

    Debug_Error(errorString);
    return nullptr;
}

bool BehaviourContainer::DeleteBehaviour(const String &behaviourName,
                                         Behaviour *behaviour,
                                         Library *openLibrary)
{
    if (!openLibrary) { return false; }

    // Get the pointer to the DeleteDynamically function
    String funcName = "DeleteDinamically_" + behaviourName;
    void (*deleteFunction)(Behaviour*) =
            (openLibrary->Get<void (*)(Behaviour*)>(funcName.ToCString()));

    if (deleteFunction)
    {
        deleteFunction(behaviour);
        return true;
    }
    else
    {
        Debug_Error(openLibrary->GetErrorString());
        return false;
    }
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
    Library *behaviourLibrary = GetBehaviourLibrary();
    if (IsLoaded())
    {
        return BehaviourContainer::CreateBehaviour(GetBehaviourName(),
                                                   behaviourLibrary);
    }
    return nullptr;
}

String BehaviourContainer::GetBehaviourName() const
{
    return GetSourceFilepath().GetName();
}

Library *BehaviourContainer::GetBehaviourLibrary() const
{
    Library *lib = BehaviourManager::GetBehaviourLib( GetSourceFilepath() );
    return lib;
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
