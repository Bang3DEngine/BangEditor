#ifndef IEVENTSEDITOR_H
#define IEVENTSEDITOR_H

#include "Bang/IEvents.h"

#include "BangEditor/BangEditor.h"

namespace Bang
{
class Path;
class GameObject;
template <class>
class EventListener;
}

namespace BangEditor
{
class EditorSettings;

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
}

#endif  // IEVENTSEDITOR_H
