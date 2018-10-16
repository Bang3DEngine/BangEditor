#ifndef PREVIEWVIEWER_H
#define PREVIEWVIEWER_H

#include <functional>
#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsFocus.h"
#include "Bang/ResourceHandle.h"
#include "Bang/String.h"
#include "Bang/Texture2D.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ResourcePreviewFactory.h"

namespace Bang {
class Texture2D;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIFocusable;
FORWARD class UIImageRenderer;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class PreviewViewer : public GameObject,
                      public EventListener<IEventsFocus>
{
    GAMEOBJECT_EDITOR(PreviewViewer);

public:
    using ImageProviderFunc = std::function<RH<Texture2D>(
                                     const ResourcePreviewFactoryParameters&)>;

	PreviewViewer();
	virtual ~PreviewViewer();

    // GameObject
    void Update() override;

    void SetPreviewImageProvider(
                    PreviewViewer::ImageProviderFunc previewImgProvider);

private:
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_border = nullptr;
    UIImageRenderer *p_imgRenderer = nullptr;
    ImageProviderFunc m_previewImageProviderFunc;
    ResourcePreviewFactoryParameters m_params;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // PREVIEWVIEWER_H

