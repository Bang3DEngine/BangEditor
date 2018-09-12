#ifndef UIINPUTARRAY_H
#define UIINPUTARRAY_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/UIInputArrayRow.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputArray : public GameObject,
                     public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(UIInputArray);

public:
    using CreateNewElementFunction = std::function<GameObject*()>;

    UIInputArray();
	virtual ~UIInputArray();

    void AddElement(GameObject *widget, uint index = -1u);
    void RemoveElement(GameObject *widget);
    void RemoveElement(uint index);

    void SetGetNewElementFunction(CreateNewElementFunction function);

    uint Size() const;

    const Array<GameObject*> &GetArray() const;

private:
    Array<GameObject*> m_array;
    GameObject *m_addNewElementRow = nullptr;

    CreateNewElementFunction m_getNewElementFunction;

    void MoveAddNewElementRowToEnd();
    void RemoveRow(UIInputArrayRow *row);
    void MoveRow(UIInputArrayRow *row, int displacement);
    UIInputArrayRow *GetRowFromElement(GameObject *element) const;

    friend class UIInputArrayRow;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTARRAY_H

