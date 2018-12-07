#include "BangEditor/SelectionProxy.h"

using namespace Bang;
using namespace BangEditor;

SelectionProxy::SelectionProxy()
{
}

SelectionProxy::~SelectionProxy()
{
}

void SelectionProxy::SetTargetGameObject(GameObject *targetGameObject)
{
    p_targetGameObject = targetGameObject;
}

GameObject *SelectionProxy::GetTargetGameObject() const
{
    return p_targetGameObject;
}
