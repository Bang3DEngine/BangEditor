#ifndef UNDOREDOCREATEGAMEOBJECT_H
#define UNDOREDOCREATEGAMEOBJECT_H

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

namespace Bang {
class GameObject;
class IEventsDestroy;
template <class > class EventEmitter;
}  // namespace Bang

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoCreateGameObject : public UndoRedoAction,
                                 public EventListener<IEventsDestroy>
{
public:
    UndoRedoCreateGameObject(GameObject *createdGameObject);
    virtual ~UndoRedoCreateGameObject();

    void Undo() override;
    void Redo() override;

private:
    GameObject *p_createdGameObject = nullptr;
    GameObject *p_parent = nullptr;
    int m_indexInParent = -1;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOCREATEGAMEOBJECT_H
