#ifndef SELECTIONOPTIONS_H
#define SELECTIONOPTIONS_H

#include "Bang/Bang.h"
#include "Bang/Component.h"
#include "Bang/AssetHandle.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace Bang
{
class Texture2D;
};

namespace BangEditor
{
class SelectionOptions : public Component
{
    COMPONENT_WITHOUT_CLASS_ID(SelectionOptions);

public:
    SelectionOptions();
    virtual ~SelectionOptions();

    void SetFilterTexture(Texture2D *filterTexture);
    void SetTargetGameObject(GameObject *targetGameObject);

    GameObject *GetTargetGameObject() const;
    Texture2D *GetFilterTexture() const;

private:
    GameObject *p_targetGameObject = nullptr;
    AH<Texture2D> p_filterTexture;
};
}

#endif  // SELECTIONOPTIONS_H
