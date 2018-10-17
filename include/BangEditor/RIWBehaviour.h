#ifndef RIWBEHAVIOUR_H
#define RIWBEHAVIOUR_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ResourceInspectorWidget.h"

namespace Bang
{
class Texture2D;
class UIButton;
class UITextRenderer;
}

using namespace Bang;
namespace BangEditor
{
class RIWBehaviour : public ResourceInspectorWidget
{
    GAMEOBJECT_EDITOR(RIWBehaviour);

public:
    // InspectorWidget
    void Init() override;

private:
    RIWBehaviour();
    virtual ~RIWBehaviour() override;

    // ResourceInspectorWidget
    void UpdateFromFileWhenChanged() override;
    Texture2D *GetIconTexture() const override;
    Color GetIconTint() const override;

    UITextRenderer *p_codeText = nullptr;
    UIButton *p_openButton = nullptr;

    friend class ResourceInspectorWidgetFactory;
};
}

#endif  // RIWBEHAVIOUR_H
