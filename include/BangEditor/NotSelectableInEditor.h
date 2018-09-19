#ifndef NOTSELECTABLEINEDITOR_H
#define NOTSELECTABLEINEDITOR_H

#include "Bang/Component.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/EditorComponentsClassIds.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class NotSelectableInEditor : public Component
{
    COMPONENT_WITH_FAST_DYNAMIC_CAST(NotSelectableInEditor)

private:
    NotSelectableInEditor()
    {
        CONSTRUCT_CLASS_ID(NotSelectableInEditor)
    }
    virtual ~NotSelectableInEditor() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // NOTSELECTABLEINEDITOR_H

