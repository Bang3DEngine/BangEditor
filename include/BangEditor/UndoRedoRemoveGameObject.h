#ifndef UNDOREDOREMOVEGAMEOBJECT_H
#define UNDOREDOREMOVEGAMEOBJECT_H

#include "Bang/Bang.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoRemoveGameObject : public UndoRedoAction
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
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOREMOVEGAMEOBJECT_H

