#include "BangEditor/SelectionOptions.h"

#include "Bang/Texture2D.h"

using namespace Bang;
using namespace BangEditor;

SelectionOptions::SelectionOptions()
{
}

SelectionOptions::~SelectionOptions()
{
}

void SelectionOptions::SetTargetGameObject(GameObject *targetGameObject)
{
    p_targetGameObject = targetGameObject;
}

void SelectionOptions::SetFilterTexture(Texture2D *filterTexture)
{
    p_filterTexture.Set(filterTexture);
}

GameObject *SelectionOptions::GetTargetGameObject() const
{
    return p_targetGameObject;
}

Texture2D *SelectionOptions::GetFilterTexture() const
{
    return p_filterTexture.Get();
}
