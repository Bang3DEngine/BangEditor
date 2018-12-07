#ifndef SELECTIONPROXY_H
#define SELECTIONPROXY_H

#include "Bang/Bang.h"
#include "Bang/Component.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace BangEditor
{
class SelectionProxy : public Component
{
    COMPONENT(SelectionProxy);

public:
    SelectionProxy();
    virtual ~SelectionProxy();

    void SetTargetGameObject(GameObject *targetGameObject);
    GameObject *GetTargetGameObject() const;

private:
    GameObject *p_targetGameObject = nullptr;
};
}

#endif  // SELECTIONPROXY_H
