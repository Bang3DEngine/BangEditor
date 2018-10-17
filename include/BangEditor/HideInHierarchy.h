#ifndef HIDEINHIERARCHY_H
#define HIDEINHIERARCHY_H

#include "Bang/Component.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/EditorComponentsClassIds.h"

using namespace Bang;
namespace BangEditor
{
class HideInHierarchy : public Component
{
    COMPONENT_WITH_FAST_DYNAMIC_CAST(HideInHierarchy)

private:
    HideInHierarchy()
    {
        CONSTRUCT_CLASS_ID(HideInHierarchy);
    }
    virtual ~HideInHierarchy() override = default;
};
}

#endif  // HIDEINHIERARCHY_H
