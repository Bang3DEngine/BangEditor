#ifndef MODELEXPLORERITEM_H
#define MODELEXPLORERITEM_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ExplorerItem.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class MenuItem;

class ModelExplorerItem : public ExplorerItem
{
    GAMEOBJECT_EDITOR(ModelExplorerItem);

public:
	ModelExplorerItem();
	virtual ~ModelExplorerItem() override;

    // UIContextMenu callback
    void OnCreateContextMenu(MenuItem *menuRootItem) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // MODELEXPLORERITEM_H

