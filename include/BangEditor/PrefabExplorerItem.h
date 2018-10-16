#ifndef PREFABEXPLORERITEM_H
#define PREFABEXPLORERITEM_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ExplorerItem.h"

namespace BangEditor {
class MenuItem;
}  // namespace BangEditor

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class PrefabExplorerItem : public ExplorerItem
{
    GAMEOBJECT_EDITOR(PrefabExplorerItem);

public:
	PrefabExplorerItem();
	virtual ~PrefabExplorerItem();

    // UIContextMenu callback
    void OnCreateContextMenu(MenuItem *menuRootItem) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // PREFABEXPLORERITEM_H

