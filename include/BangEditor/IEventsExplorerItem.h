#ifndef IEVENTSEXPLORERITEM_H
#define IEVENTSEXPLORERITEM_H

#include "Bang/IEvents.h"

#include "BangEditor/BangEditor.h"

namespace BangEditor
{
class ExplorerItem;

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
}

#endif  // IEVENTSEXPLORERITEM_H
