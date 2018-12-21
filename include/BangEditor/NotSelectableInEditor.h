#ifndef NOTSELECTABLEINEDITOR_H
#define NOTSELECTABLEINEDITOR_H

#include "Bang/Component.h"

#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace BangEditor
{
class NotSelectableInEditor : public Component
{
    COMPONENT_WITHOUT_CLASS_ID(NotSelectableInEditor)

private:
    NotSelectableInEditor() = default;
    virtual ~NotSelectableInEditor() override = default;
};
}

#endif  // NOTSELECTABLEINEDITOR_H
