#ifndef PREFABEXPLORERITEM_H
#define PREFABEXPLORERITEM_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ExplorerItem.h"

using namespace Bang;
namespace BangEditor
{
class MenuItem;

class PrefabExplorerItem : public ExplorerItem
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(PrefabExplorerItem);

public:
    PrefabExplorerItem();
    virtual ~PrefabExplorerItem() override;

    // UIContextMenu callback
    void OnCreateContextMenu(MenuItem *menuRootItem) override;
};
}

#endif  // PREFABEXPLORERITEM_H
