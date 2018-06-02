#ifndef UNDOREDOREMOVEGAMEOBJECT_H
#define UNDOREDOREMOVEGAMEOBJECT_H

#include "Bang/Bang.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/IEventsDestroy.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoRemoveGameObject : public UndoRedoAction,
                                 public EventListener<IEventsDestroy>
{
public:
    UndoRedoRemoveGameObject(GameObject *removedGameObject);
    virtual ~UndoRedoRemoveGameObject();

    void Undo() override;
    void Redo() override;

private:
    GameObject *p_removedGameObject = nullptr;
    GameObject *p_previousParent = nullptr;
    int m_indexInPreviousParent = -1;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOREMOVEGAMEOBJECT_H

