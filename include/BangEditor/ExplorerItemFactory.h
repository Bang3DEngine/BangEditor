#ifndef EXPLORERITEMFACTORY_H
#define EXPLORERITEMFACTORY_H

#include "Bang/Array.h"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Path;
}

using namespace Bang;
namespace BangEditor
{
class ExplorerItem;

class ExplorerItemFactory
{
public:
    static ExplorerItem *CreateExplorerItem(const Path &path);
    static Array<ExplorerItem *> CreateAndGetSubPathsExplorerItems(
        const Path &path,
        bool addBackItem,
        bool recursive = false);
    static bool CanHaveSubpaths(const Path &path);

private:
    ExplorerItemFactory();
    virtual ~ExplorerItemFactory();
};
}

#endif  // EXPLORERITEMFACTORY_H
