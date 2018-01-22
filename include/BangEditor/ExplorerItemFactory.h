#ifndef EXPLORERITEMFACTORY_H
#define EXPLORERITEMFACTORY_H

#include "Bang/Bang.h"
#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ExplorerItem;

class ExplorerItemFactory
{
public:
    static ExplorerItem *CreateExplorerItem(const Path &path);
    static List<ExplorerItem*> CreateAndGetChildrenExplorerItems(
                                                    const Path &path);
    static bool CanHaveChildren(const Path &path);

private:
	ExplorerItemFactory();
	virtual ~ExplorerItemFactory();
};

NAMESPACE_BANG_EDITOR_END

#endif // EXPLORERITEMFACTORY_H

