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
                     public EventListener<IEventsValueChanged>,
                     public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(UIInputArray);

public:
    UIInputArray();
	virtual ~UIInputArray();

    void AddRow(GameObject *rowGameObject, uint index = -1u);
    void RemoveRow(GameObject *rowGameObject);
    void RemoveRow(uint index);

    using CreateNewRowGameObjectFunction = std::function<GameObject*()>;
    void SetCreateNewRowGameObjectFunction(std::function<GameObject*()> function);

    template <class T>
    void UpdateRows(const Array<T*> &referenceSerializables);

    template <class T>
    void UpdateReferences(const Array<T*> &referenceSerializables,
                          std::function<T*()> createNewReferenceFunction,
                          std::function<void(T*)> removeReferenceFunction);

    void Clear();
    uint Size() const;

    const Array<GameObject*> &GetRowGameObjects() const;

private:
    bool m_updatingRowsOrReferences = false;
    Array<GameObject*> m_rowGameObjects;
    GameObject *m_addNewRowButtonRow = nullptr;

    CreateNewRowGameObjectFunction m_createNewRowGameObjectFunction;

    void AddRow_(GameObject *rowGameObject, uint index, bool propagateChangeEvent);
    void RemoveRow_(GameObject *rowGameObject, bool propagateChangeEvent);

    void UpdateSerializables(
               Array<Serializable*> serializablesToUpdate,
               Array<Serializable*> serializablesToUpdateFrom,
               std::function<Serializable*()> createNewSerializableFunction,
               std::function<void(Serializable*)> removeSerializableFunction,
               bool newCreatedSerializablesUpdateTheFromSerializable);

    void MoveAddNewRowButtonRowToEnd();
    void MoveRow(UIInputArrayRow *row, int displacement);
    void RemoveRow(UIInputArrayRow *row, bool propagateChangeEvent);
    UIInputArrayRow *GetRowFromRowGameObject(GameObject *rowGameObject) const;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

    friend class UIInputArrayRow;
};

template <class T>
void UIInputArray::UpdateRows(const Array<T*> &referenceSerializables)
{
    if (!m_updatingRowsOrReferences)
    {
        m_updatingRowsOrReferences = true;
        auto rowGameObjectsCasted =
                GetRowGameObjects().template To<Array, Serializable*>();
        auto referenceSerializablesCasted =
                referenceSerializables.template To<Array, Serializable*>();
        UpdateSerializables(
                    rowGameObjectsCasted,
                    referenceSerializablesCasted,
                    [this]()
                    {
                        GameObject *newObj = m_createNewRowGameObjectFunction();
                        AddRow_(newObj, -1u, false);
                        return SCAST<Serializable*>(newObj);
                    },
                    [this](Serializable *s)
                    {
                        RemoveRow_(SCAST<GameObject*>(s), false);
                    },
                    false);
        m_updatingRowsOrReferences = false;
    }
}

template <class T>
void UIInputArray::UpdateReferences(const Array<T*> &referenceSerializables,
                                    std::function<T*()> createNewReferenceFunction,
                                    std::function<void(T*)> removeReferenceFunction)
{
    if (!m_updatingRowsOrReferences)
    {
        m_updatingRowsOrReferences = true;
        auto rowGameObjectsCasted =
                GetRowGameObjects().template To<Array, Serializable*>();
        auto referenceSerializablesCasted =
                referenceSerializables.template To<Array, Serializable*>();
        UpdateSerializables(
                    referenceSerializablesCasted,
                    rowGameObjectsCasted,
                    [&]()
                    {
                        T *newObj = createNewReferenceFunction();
                        return SCAST<Serializable*>(newObj);
                    },
                    [&](Serializable *s)
                    {
                        removeReferenceFunction(SCAST<T*>(s));
                    },
                    true);
        m_updatingRowsOrReferences = false;
    }
}

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTARRAY_H

