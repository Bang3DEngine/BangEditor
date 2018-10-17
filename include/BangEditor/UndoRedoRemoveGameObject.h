#ifndef UNDOREDOREMOVEGAMEOBJECT_H
#define UNDOREDOREMOVEGAMEOBJECT_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDestroy.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"

namespace Bang
{
class GameObject;
class IEventsDestroy;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UndoRedoRemoveGameObject : public UndoRedoAction,
                                 public EventListener<IEventsDestroy>
{
public:
    UndoRedoRemoveGameObject(GameObject *removedGameObject);
    virtual ~UndoRedoRemoveGameObject() override;

    void Undo() override;
    void Redo() override;

private:
    GameObject *p_removedGameObject = nullptr;
    GameObject *p_previousParent = nullptr;
    int m_indexInPreviousParent = -1;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};
}

#endif  // UNDOREDOREMOVEGAMEOBJECT_H
