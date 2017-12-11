#ifndef HIDEINHIERARCHY_H
#define HIDEINHIERARCHY_H

#include "Bang/Component.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class HideInHierarchy : public Component
{
    COMPONENT(Component)

private:
    HideInHierarchy() = default;
    virtual ~HideInHierarchy() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // HIDEINHIERARCHY_H

