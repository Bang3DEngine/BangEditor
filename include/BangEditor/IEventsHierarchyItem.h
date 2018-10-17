#ifndef IEVENTSHIERARCHYITEM_H
#define IEVENTSHIERARCHYITEM_H

#include "Bang/IEvents.h"

#include "BangEditor/BangEditor.h"

namespace BangEditor
{
class HierarchyItem;

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
}

#endif  // IEVENTSHIERARCHYITEM_H
