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

namespace Bang
{
class IEventsDestroy;
class GameObject;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
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
}

#endif  // UNDOREDOGAMEOBJECTSELECTION_H
