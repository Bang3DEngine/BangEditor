#ifndef PREVIEWVIEWER_H
#define PREVIEWVIEWER_H

#include <functional>

#include "Bang/Bang.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsFocus.h"
#include "Bang/ResourceHandle.h"

#include "BangEditor/ResourcePreviewFactory.h"

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

