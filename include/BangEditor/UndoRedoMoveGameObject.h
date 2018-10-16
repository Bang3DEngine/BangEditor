#ifndef UNDOREDOMOVEGAMEOBJECT_H
#define UNDOREDOMOVEGAMEOBJECT_H

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

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class GameObject;
FORWARD   class IEventsDestroy;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoMoveGameObject : public UndoRedoAction,
                               public EventListener<IEventsDestroy>
{
public:
    UndoRedoMoveGameObject(GameObject *movedGameObject,
                           GameObject *previousParent,
                           int indexInPreviousParent,
                           GameObject *newParent,
                           int indexInNewParent);
    virtual ~UndoRedoMoveGameObject() override;

    void Undo() override;
    void Redo() override;
    bool IsRedundant() const override;

private:
    GameObject *p_movedGameObject = nullptr;

    GameObject *p_previousParent = nullptr;
    int m_indexInPreviousParent = -1;

    GameObject *p_newParent = nullptr;
    int m_indexInNewParent = -1;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOMOVEGAMEOBJECT_H

