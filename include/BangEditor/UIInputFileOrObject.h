#ifndef UIINPUTFILEOROBJECT_H
#define UIINPUTFILEOROBJECT_H

#include "Bang/Array.h"
#include "Bang/Array.tcc"
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
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsDragDrop;
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
                            public EventListener<IEventsDragDrop>
{
    GAMEOBJECT_EDITOR(UIInputFileOrObject);

public:
    UIInputFileOrObject();
    virtual ~UIInputFileOrObject() override;

    UIInputText *GetInputText() const;
    UIButton *GetSearchButton() const;
    UIButton *GetOpenButton() const;
    const Array<String> &GetExtensions() const;

protected:
    virtual bool AcceptsDrag(
        EventEmitter<IEventsDragDrop> *dragDroppable) const;
    virtual void OnDropped(EventEmitter<IEventsDragDrop> *dragDroppable);
    virtual void OnSearchButtonClicked();
    virtual void OnOpenButtonClicked();

    // IEventsDragDrop
    virtual void OnDragStarted(
        EventEmitter<IEventsDragDrop> *dragDroppable) override;
    virtual void OnDragUpdate(
        EventEmitter<IEventsDragDrop> *dragDroppable) override;
    virtual void OnDrop(EventEmitter<IEventsDragDrop> *dragDroppable) override;

private:
    UIButton *p_searchButton = nullptr;
    UIInputText *p_inputText = nullptr;
    UIButton *p_openButton = nullptr;
};
}

#endif  // UIINPUTFILEOROBJECT_H
