#ifndef IEVENTSPROJECTMANAGER_H
#define IEVENTSPROJECTMANAGER_H

#include "Bang/IEvents.h"

#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class Project;

class IEventsProjectManager
{
    IEVENTS_NS(IEventsProjectManager);

public:
    virtual void OnProjectOpen(const Project *project)
    {
        (void) project;
    }

    virtual void OnProjectClosed(const Project *project)
    {
        (void) project;
    }
};

NAMESPACE_BANG_EDITOR_END

#endif // IEVENTSPROJECTMANAGER_H
