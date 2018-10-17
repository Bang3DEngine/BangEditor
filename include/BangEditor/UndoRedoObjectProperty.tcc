#pragma once

#include "BangEditor/UndoRedoObjectProperty.h"

using namespace Bang;
namespace BangEditor
{
template <class T>
UndoRedoObjectProperty<T>::UndoRedoObjectProperty(
    Object *obj,
    const T &prevValue,
    const T &newValue,
    UndoRedoObjectProperty<T>::SetterFunc setter)
{
    p_obj = obj;
    p_obj->EventEmitter<IEventsDestroy>::RegisterListener(this);

    m_prevValue = prevValue;
    m_newValue = newValue;

    m_setter = setter;
}

template <class T>
void UndoRedoObjectProperty<T>::Undo()
{
    if(p_obj)
    {
        m_setter(p_obj, m_prevValue);
    }
}

template <class T>
void UndoRedoObjectProperty<T>::Redo()
{
    if(p_obj)
    {
        m_setter(p_obj, m_newValue);
    }
}

template <class T>
bool UndoRedoObjectProperty<T>::IsRedundant() const
{
    return (m_prevValue == m_newValue);
}

template <class T>
void UndoRedoObjectProperty<T>::OnDestroyed(
    EventEmitter<IEventsDestroy> *object)
{
    ASSERT(p_obj == object);
    p_obj = nullptr;
}
}
