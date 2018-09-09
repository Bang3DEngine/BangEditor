#ifndef IEVENTSHIERARCHYITEM_H
#define IEVENTSHIERARCHYITEM_H

#include "Bang/IEvents.h"

#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class HierarchyItem;

class IEventsHierarchyItem
{
    IEVENTS_NS(IEventsHierarchyItem);

public:
    virtual void OnRename(HierarchyItem *item)
    {
        BANG_UNUSED(item);
    }

    virtual void OnRemove(HierarchyItem *item)
    {
        BANG_UNUSED(item);
    }

    virtual void OnCopy(HierarchyItem *item)
    {
        BANG_UNUSED(item);
    }

    virtual void OnCut(HierarchyItem *item)
    {
        BANG_UNUSED(item);
    }

    virtual void OnPaste(HierarchyItem *item)
    {
        BANG_UNUSED(item);
    }

    virtual void OnDuplicate(HierarchyItem *item)
    {
        BANG_UNUSED(item);
    }

    virtual void OnCreatePrefab(HierarchyItem *item)
    {
        BANG_UNUSED(item);
    }

};

NAMESPACE_BANG_EDITOR_END

#endif // IEVENTSHIERARCHYITEM_H
