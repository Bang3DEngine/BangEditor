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

    void SetAcceptedClassIdBegin(ClassIdType classIdBegin);
    void SetAcceptedClassIdEnd(ClassIdType classIdEnd);
    void SetObject(Object *object);

    ClassIdType GetAcceptedClassIdBegin() const;
    ClassIdType GetAcceptedClassIdEnd() const;
    Object *GetObject() const;

protected:
    virtual ~UIInputObject() override;

    // UIInputFileOrObject
    bool CanDoZoom() const override;
    bool AcceptsDrag(
        EventEmitter<IEventsDragDrop> *dragDroppable) const override;
    void OnDropped(EventEmitter<IEventsDragDrop> *dragDroppable) override;
    void OnSearchButtonClicked() override;
    void OnOpenButtonClicked() override;

private:
    ClassIdType m_acceptedClassIdBegin = -1u;
    ClassIdType m_acceptedClassIdEnd = -1u;
    Object *p_object = nullptr;
};

template <class T>
class UIInputObjectT : public UIInputObject
{
};
}

#endif  // UIINPUTOBJECT_H
