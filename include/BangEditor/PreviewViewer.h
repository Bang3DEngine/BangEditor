#ifndef PREVIEWVIEWER_H
#define PREVIEWVIEWER_H

#include <functional>
#include <vector>

#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsFocus.h"
#include "Bang/String.h"
#include "Bang/Texture2D.h"
#include "BangEditor/AssetPreviewFactory.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Texture2D;
class UIFocusable;
class UIImageRenderer;
}

using namespace Bang;
namespace BangEditor
{
class PreviewViewer : public GameObject, public EventListener<IEventsFocus>
{
    GAMEOBJECT_EDITOR(PreviewViewer);

public:
    using ImageProviderFunc =
        std::function<AH<Texture2D>(const AssetPreviewFactoryParameters &)>;

    PreviewViewer();
    virtual ~PreviewViewer() override;

    // GameObject
    void Update() override;

    void SetPreviewImageProvider(
        PreviewViewer::ImageProviderFunc previewImgProvider);

private:
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_border = nullptr;
    UIImageRenderer *p_imgRenderer = nullptr;
    ImageProviderFunc m_previewImageProviderFunc;
    AssetPreviewFactoryParameters m_params;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;
};
}

#endif  // PREVIEWVIEWER_H
