#ifndef IEVENTSEXPLORERITEM_H
#define IEVENTSEXPLORERITEM_H

#include "Bang/IEvents.h"

#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ExplorerItem;

class IEventsExplorerItem
{
    IEVENTS_NS(IEventsExplorerItem);

public:
    virtual void OnRename(ExplorerItem *item) = 0;
    virtual void OnRemove(ExplorerItem *item) = 0;
    virtual void OnDuplicate(ExplorerItem *item) = 0;
    virtual void OnPastedOver(ExplorerItem *item) = 0;
    virtual void OnDroppedToDirectory(ExplorerItem *item) = 0;
};

NAMESPACE_BANG_EDITOR_END

#endif // IEVENTSEXPLORERITEM_H
