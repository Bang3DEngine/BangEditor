#ifndef UIINPUTFILEWITHPREVIEW_H
#define UIINPUTFILEWITHPREVIEW_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsFocus.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Texture2D.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/UIInputFile.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Path;
FORWARD class Resource;
FORWARD class Texture2D;
FORWARD class UIFocusable;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputFileWithPreview : public UIInputFile,
                               public EventListener<IEventsFocus>
{
public:
	UIInputFileWithPreview();
	virtual ~UIInputFileWithPreview() override;

    void SetZoomable(bool zoomable);
    void SetResource(Resource *resource);
    void SetPath(const Path &path) override;
    virtual RH<Texture2D> GetPreviewTextureFromPath(const Path &path);

private:
    bool m_zoomable = true;
    UIImageRenderer *p_previewImg = nullptr;
    UIImageRenderer *p_bigPreviewImg = nullptr;

    bool HasExistingPath() const;

    // IFocusable
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTFILEWITHPREVIEW_H

