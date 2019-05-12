#ifndef AIWBEHAVIOUR_H
#define AIWBEHAVIOUR_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "BangMath/Color.h"
#include "Bang/String.h"
#include "BangEditor/AssetInspectorWidget.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Texture2D;
class UIButton;
class UITextRenderer;
}

using namespace Bang;
namespace BangEditor
{
class AIWBehaviour : public AssetInspectorWidget
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(AIWBehaviour);

public:
    // InspectorWidget
    void Init() override;

private:
    AIWBehaviour();
    virtual ~AIWBehaviour() override;

    // AssetInspectorWidget
    void UpdateFromFileWhenChanged() override;
    Texture2D *GetIconTexture() const override;
    Color GetIconTint() const override;

    UITextRenderer *p_codeText = nullptr;
    UIButton *p_openButton = nullptr;

    friend class AssetInspectorWidgetFactory;
};
}

#endif  // AIWBEHAVIOUR_H
