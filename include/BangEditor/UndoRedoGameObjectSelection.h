#ifndef UNDOREDOGAMEOBJECTSELECTION_H
#define UNDOREDOGAMEOBJECTSELECTION_H

#include "Bang/Bang.h"
#include "Bang/Path.h"
#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoGameObjectSelection : public UndoRedoAction
{
public:
    UndoRedoGameObjectSelection(GameObject *previousSelectedGameObject,
                                GameObject *newSelectedGameObject);
    virtual ~UndoRedoGameObjectSelection();

    // UndoRedoAction
    void Undo() override;
    void Redo() override;

private:
    GameObject *p_previousSelectedGameObject = nullptr;
    GameObject *p_newSelectedGameObject = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOGAMEOBJECTSELECTION_H

