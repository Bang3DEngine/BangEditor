#ifndef PREVIEWVIEWER_H
#define PREVIEWVIEWER_H

#include <functional>

#include "Bang/Bang.h"
#include "Bang/Texture2D.h"
#include "Bang/ResourceHandle.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class PreviewViewer
{
public:
    using ImageProviderFunc = std::function<RH<Texture2D>()>;

	PreviewViewer();
	virtual ~PreviewViewer();

    void SetPreviewImageProvider(
                    PreviewViewer::ImageProviderFunc previewImgProvider);

private:
    ImageProviderFunc m_previewImageProviderFunc;
};

NAMESPACE_BANG_EDITOR_END

#endif // PREVIEWVIEWER_H

