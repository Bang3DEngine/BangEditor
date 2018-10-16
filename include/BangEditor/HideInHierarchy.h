#ifndef HIDEINHIERARCHY_H
#define HIDEINHIERARCHY_H

#include "Bang/Component.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/EditorComponentsClassIds.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

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

NAMESPACE_BANG_EDITOR_END

#endif // HIDEINHIERARCHY_H

