#include "BangEditor/PreviewViewer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

PreviewViewer::PreviewViewer()
{
}

PreviewViewer::~PreviewViewer()
{
}

void PreviewViewer::SetPreviewImageProvider(
                            PreviewViewer::ImageProviderFunc previewImgProvider)
{
    m_previewImageProviderFunc = previewImgProvider;
}

