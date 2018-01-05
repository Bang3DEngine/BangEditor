#ifndef BEHAVIOURCONTAINER_H
#define BEHAVIOURCONTAINER_H

#include "Bang/Path.h"
#include "Bang/Component.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Library;
FORWARD class XMLNode;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class Behaviour;

class BehaviourContainer : public Component
{
    COMPONENT(BehaviourContainer)

public:
    void SetSourceFilepath(const Path &sourceFilepath);
    Behaviour* CreateBehaviourInstance() const;

    String GetBehaviourName() const;
    Library *GetBehaviourLibrary() const;
    const Path& GetSourceFilepath() const;
    bool IsLoaded() const;

    static Behaviour* CreateBehaviour(const String &behaviourName,
                                      Library *openLibrary);
    static bool DeleteBehaviour(const String &behaviourName,
                                Behaviour *behaviour,
                                Library *openLibrary);
private:
    Path m_sourceFilepath;
    Behaviour *p_behaviour = nullptr;

    BehaviourContainer();
    virtual ~BehaviourContainer();

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    friend class EditorComponentFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // BEHAVIOURCONTAINER_H

