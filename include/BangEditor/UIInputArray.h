#ifndef UIINPUTARRAY_H
#define UIINPUTARRAY_H

#include <sys/types.h>
#include <functional>
#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/UIInputArrayRow.h"

namespace Bang
{
class IEventsValueChanged;
class Serializable;
}

using namespace Bang;
namespace BangEditor
{
class UIInputArrayRow;

// The GameObject's in each row will use MetaNode's to serialize/deserialize
// and update array elements
class UIInputArray : public GameObject,
                     public EventListener<IEventsValueChanged>,
                     public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(UIInputArray);

public:
    UIInputArray();
    virtual ~UIInputArray() override;

    void AddRow(GameObject *rowGameObject, uint index = -1u);
    void RemoveRow(GameObject *rowGameObject);
    void RemoveRow(uint index);

    using CreateNewRowGameObjectFunction = std::function<GameObject *()>;
    void SetCreateNewRowGameObjectFunction(
        std::function<GameObject *()> function);

    template <class T>
    void UpdateRows(const Array<T *> &referenceSerializables);

    template <class T>
    void UpdateReferences(const Array<T *> &referenceSerializables,
                          std::function<T *()> createNewReferenceFunction,
                          std::function<void(T *)> removeReferenceFunction);

    void Clear(bool synchronize = false);
    uint Size() const;

    const Array<GameObject *> &GetRowGameObjects() const;

private:
    bool m_updatingRowsOrReferences = false;
    Array<GameObject *> m_rowGameObjects;
    GameObject *m_addNewRowButtonRow = nullptr;

    CreateNewRowGameObjectFunction m_createNewRowGameObjectFunction;

    void AddRow_(GameObject *rowGameObject,
                 uint index,
                 bool propagateChangeEvent);
    void RemoveRow_(GameObject *rowGameObject, bool propagateChangeEvent);

    void UpdateSerializables(
        Array<Serializable *> serializablesToUpdate,
        Array<Serializable *> serializablesToUpdateFrom,
        std::function<Serializable *()> createNewSerializableFunction,
        std::function<void(Serializable *)> removeSerializableFunction,
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
void UIInputArray::UpdateRows(const Array<T *> &referenceSerializables)
{
    if (!m_updatingRowsOrReferences)
    {
        m_updatingRowsOrReferences = true;
        auto rowGameObjectsCasted =
            GetRowGameObjects().template To<Array, Serializable *>();
        auto referenceSerializablesCasted =
            referenceSerializables.template To<Array, Serializable *>();
        UpdateSerializables(rowGameObjectsCasted,
                            referenceSerializablesCasted,
                            [this]() {
                                GameObject *newObj =
                                    m_createNewRowGameObjectFunction();
                                AddRow_(newObj, -1u, false);
                                return SCAST<Serializable *>(newObj);
                            },
                            [this](Serializable *s) {
                                RemoveRow_(SCAST<GameObject *>(s), false);
                            },
                            false);
        m_updatingRowsOrReferences = false;
    }
}

template <class T>
void UIInputArray::UpdateReferences(
    const Array<T *> &referenceSerializables,
    std::function<T *()> createNewReferenceFunction,
    std::function<void(T *)> removeReferenceFunction)
{
    if (!m_updatingRowsOrReferences)
    {
        m_updatingRowsOrReferences = true;
        auto rowGameObjectsCasted =
            GetRowGameObjects().template To<Array, Serializable *>();
        auto referenceSerializablesCasted =
            referenceSerializables.template To<Array, Serializable *>();
        UpdateSerializables(
            referenceSerializablesCasted,
            rowGameObjectsCasted,
            [&]() {
                T *newObj = createNewReferenceFunction();
                return SCAST<Serializable *>(newObj);
            },
            [&](Serializable *s) { removeReferenceFunction(SCAST<T *>(s)); },
            true);
        m_updatingRowsOrReferences = false;
    }
}
}

#endif  // UIINPUTARRAY_H
