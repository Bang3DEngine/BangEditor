#ifndef MODELEXPLORERITEM_H
#define MODELEXPLORERITEM_H

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

class ModelExplorerItem : public ExplorerItem
{
    GAMEOBJECT_EDITOR(ModelExplorerItem);

public:
	ModelExplorerItem();
	virtual ~ModelExplorerItem();

    // UIContextMenu callback
    void OnCreateContextMenu(MenuItem *menuRootItem) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // MODELEXPLORERITEM_H

