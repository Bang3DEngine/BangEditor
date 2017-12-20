#ifndef NOTSELECTABLEINEDITOR_H
#define NOTSELECTABLEINEDITOR_H

#include "Bang/Component.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class NotSelectableInEditor : public Component
{
    COMPONENT(NotSelectableInEditor)

private:
    NotSelectableInEditor() = default;
    virtual ~NotSelectableInEditor() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // NOTSELECTABLEINEDITOR_H

