#ifndef UIINPUTFILEOROBJECT_H
#define UIINPUTFILEOROBJECT_H

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "Bang/UIFocusable.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsDragDrop;
class UIEvent;
class UIFocusable;
class UIImageRenderer;
class Texture2D;
class IEventsValueChanged;
class UIButton;
class UIInputNumber;
class UIInputText;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFileOrObject : public GameObject,
                            public EventEmitter<IEventsValueChanged>,
                            public EventListener<IEventsDragDrop>,
                            public EventListener<IEventsFocus>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(UIInputFileOrObject);

public:
    UIInputFileOrObject();
    virtual ~UIInputFileOrObject() override;

    void SetZoomable(bool zoomable);
    void SetPreviewIcon(Texture2D *icon, const Color &tint = Color::White());
    void SetShowPreview(bool showPreview);

    UIImageRenderer *GetPreviewIcon() const;
    UIImageRenderer *GetBigPreviewIcon() const;
    UIInputText *GetInputText() const;
    UIButton *GetSearchButton() const;
    UIButton *GetOpenButton() const;
    bool GetShowPreview() const;
    const Array<String> &GetExtensions() const;

protected:
    virtual bool AcceptsDrag(
        EventEmitter<IEventsDragDrop> *dragDroppable) const;
    virtual void OnDropped(EventEmitter<IEventsDragDrop> *dragDroppable);
    virtual void OnSearchButtonClicked();
    virtual void OnOpenButtonClicked();
    virtual bool CanDoZoom() const;

    // IFocusable
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

    // IEventsDragDrop
    virtual void OnDragStarted(
        EventEmitter<IEventsDragDrop> *dragDroppable) override;
    virtual void OnDragUpdate(
        EventEmitter<IEventsDragDrop> *dragDroppable) override;
    virtual void OnDrop(EventEmitter<IEventsDragDrop> *dragDroppable) override;

private:
    bool m_zoomable = true;
    bool m_showPreview = true;

    UIButton *p_searchButton = nullptr;
    UIInputText *p_inputText = nullptr;
    UIButton *p_openButton = nullptr;
    UIImageRenderer *p_previewIcon = nullptr;
    UIImageRenderer *p_bigPreviewIcon = nullptr;
};
}

#endif  // UIINPUTFILEOROBJECT_H
