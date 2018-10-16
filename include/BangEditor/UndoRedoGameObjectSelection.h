#ifndef UNDOREDOGAMEOBJECTSELECTION_H
#define UNDOREDOGAMEOBJECTSELECTION_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/Path.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class IEventsDestroy;
FORWARD   class GameObject;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoGameObjectSelection : public UndoRedoAction,
                                    public EventListener<IEventsDestroy>
{
public:
    UndoRedoGameObjectSelection(GameObject *previousSelectedGameObject,
                                GameObject *newSelectedGameObject);
    virtual ~UndoRedoGameObjectSelection() override;

    // UndoRedoAction
    void Undo() override;
    void Redo() override;
    bool IsRedundant() const override;

private:
    GameObject *p_previousSelectedGameObject = nullptr;
    GameObject *p_newSelectedGameObject = nullptr;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOGAMEOBJECTSELECTION_H

