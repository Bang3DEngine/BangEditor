#ifndef UNDOREDOOBJECTPROPERTY_H
#define UNDOREDOOBJECTPROPERTY_H

#include "Bang/Bang.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/Object.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"

using namespace Bang;
namespace BangEditor
{
template <class T>
class UndoRedoObjectProperty : public UndoRedoAction,
                               public EventListener<IEventsDestroy>
{
public:
    using SetterFunc = std::function<void(Object *obj, const T &value)>;

    UndoRedoObjectProperty(Object *obj,
                           const T &prevValue,
                           const T &newValue,
                           SetterFunc setter);
    virtual ~UndoRedoObjectProperty() = default;

    // UndoRedoAction
    void Undo() override;
    void Redo() override;
    bool IsRedundant() const override;

    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

private:
    T m_prevValue;
    T m_newValue;
    SetterFunc m_setter;
    Object *p_obj = nullptr;
};
}

#include "BangEditor/UndoRedoObjectProperty.tcc"

#endif  // UNDOREDOOBJECTPROPERTY_H
