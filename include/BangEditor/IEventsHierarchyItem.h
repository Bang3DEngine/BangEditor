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
    virtual void OnCreateEmpty(HierarchyItem *item) = 0;
    virtual void OnRename(HierarchyItem *item) = 0;
    virtual void OnRemove(HierarchyItem *item) = 0;
    virtual void OnCopy(HierarchyItem *item) = 0;
    virtual void OnCut(HierarchyItem *item) = 0;
    virtual void OnPaste(HierarchyItem *item) = 0;
    virtual void OnDuplicate(HierarchyItem *item) = 0;
    virtual void OnCreatePrefab(HierarchyItem *item) = 0;
};

NAMESPACE_BANG_EDITOR_END

#endif // IEVENTSHIERARCHYITEM_H
