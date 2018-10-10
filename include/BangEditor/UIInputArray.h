#ifndef UIINPUTARRAY_H
#define UIINPUTARRAY_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/UIInputArrayRow.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

// The GameObject's in each row will use MetaNode's to serialize/deserialize
// and update array elements
class UIInputArray : public GameObject,
                     public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(UIInputArray);

public:
    UIInputArray();
	virtual ~UIInputArray();

    void AddInputGameObject(GameObject *widget, uint index = -1u);
    void UpdateInputGameObjects(const Array<MetaNode> &elementsMeta);
    void RemoveInputGameObjectSerializable(Serializable *widget);
    void RemoveInputGameObject(GameObject *widget);
    void RemoveInputGameObject(uint index);

    using CreateNewInputGameObjectFunction = std::function<GameObject*()>;
    void SetCreateNewInputGameObjectFunction(std::function<GameObject*()> function);

    template <class T>
    static void UpdateElements(
                    const Array<T*> &referenceMetaNodes,
                    const Array<T*> &serializablesToUpdate,
                    std::function<T*()> createNewElementFunction,
                    std::function<void(T*)> removeElementFunction);

    static void UpdateElementsSerializable(
                    const Array<MetaNode> &referenceMetaNodes,
                    const Array<Serializable*> &serializablesToUpdate,
                    std::function<Serializable*()> createNewElementFunction,
                    std::function<void(Serializable*)> removeElementFunction);

    void Clear();
    uint Size() const;

    Array<MetaNode> GetInputGameObjectsMetas() const;
    const Array<GameObject*> &GetInputGameObjectsArray() const;

private:
    Array<GameObject*> m_inputGameObjects;
    GameObject *m_addNewElementRow = nullptr;

    CreateNewInputGameObjectFunction m_createNewInputGameObjectFunction;

    void AddInputGameObject_(GameObject *widget, uint index, bool propagateChangeEvent);
    void RemoveInputGameObject_(GameObject *widget, bool propagateChangeEvent);

    void MoveAddNewInputGameObjectRowToEnd();
    void RemoveRow(UIInputArrayRow *row, bool propagateChangeEvent);
    void MoveRow(UIInputArrayRow *row, int displacement);
    UIInputArrayRow *GetRowFromInputGameObject(GameObject *element) const;

    friend class UIInputArrayRow;
};

template <class T>
void UIInputArray::UpdateElements(
                const Array<MetaNode> &referenceMetaNodes,
                const Array<T*> &serializablesToUpdate,
                std::function<T*()> createNewElementFunction,
                std::function<void(T*)> removeElementFunction)
{
    UIInputArray::UpdateElementsSerializable(
                referenceMetaNodes,
                serializablesToUpdate.template To<Array, Serializable*>(),
                [&]()
                {
                    T *newObj = createNewElementFunction();
                    return SCAST<Serializable*>(newObj);
                },
                [&](Serializable *s)
                {
                    removeElementFunction(SCAST<T*>(s));
                });
}

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTARRAY_H

