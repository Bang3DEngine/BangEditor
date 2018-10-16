#ifndef RIWBEHAVIOUR_H
#define RIWBEHAVIOUR_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ResourceInspectorWidget.h"

namespace Bang {
class Texture2D;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD class UITextRenderer;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class RIWBehaviour : public ResourceInspectorWidget
{
    GAMEOBJECT_EDITOR(RIWBehaviour);

public:
    // InspectorWidget
    void Init() override;

private:
	RIWBehaviour();
	virtual ~RIWBehaviour();

    // ResourceInspectorWidget
    void UpdateFromFileWhenChanged() override;
    Texture2D *GetIconTexture() const override;
    Color GetIconTint() const override;

    UITextRenderer *p_codeText = nullptr;
    UIButton *p_openButton = nullptr;

    friend class ResourceInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // RIWBEHAVIOUR_H

