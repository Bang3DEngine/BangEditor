#ifndef IEVENTSEDITOR_H
#define IEVENTSEDITOR_H

#include "Bang/IEvents.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class Path;
FORWARD   class GameObject;
FORWARD_T class EventListener;
FORWARD NAMESPACE_BANG_END

NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class EditorSettings;

class IEventsEditor
{
    IEVENTS_NS(IEventsEditor);

public:
    virtual void OnGameObjectSelected(Bang::GameObject *selectedGameObject)
    {
        BANG_UNUSED(selectedGameObject);
    }

    virtual void OnExplorerPathSelected(const Bang::Path &selectedPath)
    {
        BANG_UNUSED(selectedPath);
    }
};

NAMESPACE_BANG_EDITOR_END

#endif // IEVENTSEDITOR_H
