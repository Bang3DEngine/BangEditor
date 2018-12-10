#ifndef UIINPUTOBJECT_H
#define UIINPUTOBJECT_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/String.h"
#include "BangEditor/UIInputFileOrObject.h"

namespace Bang
{
class Object;
class UIInputText;
}

using namespace Bang;
namespace BangEditor
{
class UIInputObject : public UIInputFileOrObject
{
    GAMEOBJECT_EDITOR(UIInputObject);

public:
    UIInputObject();

    void SetObject(Object *object);

    Object *GetObject() const;

protected:
    virtual ~UIInputObject() override;

    // UIInputFileOrObject
    bool AcceptsDrag(
        EventEmitter<IEventsDragDrop> *dragDroppable) const override;
    void OnDropped(EventEmitter<IEventsDragDrop> *dragDroppable) override;
    void OnSearchButtonClicked() override;
    void OnOpenButtonClicked() override;

private:
    Object *p_object = nullptr;
};

template <class T>
class UIInputObjectT : public UIInputObject
{
};
}

#endif  // UIINPUTOBJECT_H
