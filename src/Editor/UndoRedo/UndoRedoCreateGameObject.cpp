#include "BangEditor/UndoRedoCreateGameObject.h"

#include "Bang/Assert.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEventsDestroy.h"
#include "BangEditor/Editor.h"

using namespace Bang;
using namespace BangEditor;

UndoRedoCreateGameObject::UndoRedoCreateGameObject(
    GameObject *createdGameObject)
{
    p_createdGameObject = createdGameObject;
    ASSERT(p_createdGameObject);

    p_parent = createdGameObject->GetParent();
    ASSERT(p_parent);

    m_indexInParent = createdGameObject->GetIndexInsideParent();

    p_parent->EventEmitter<IEventsDestroy>::RegisterListener(this);
    p_createdGameObject->EventEmitter<IEventsDestroy>::RegisterListener(this);
}

UndoRedoCreateGameObject::~UndoRedoCreateGameObject()
{
    if (p_createdGameObject && p_createdGameObject->GetParent() == nullptr)
    {
        GameObject::Destroy(p_createdGameObject);
    }
}

void UndoRedoCreateGameObject::Undo()
{
    if (p_createdGameObject)
    {
        p_createdGameObject->SetParent(nullptr);
    }
}

void UndoRedoCreateGameObject::Redo()
{
    if (p_createdGameObject)
    {
        p_createdGameObject->SetParent(p_parent, m_indexInParent);
        Editor::SelectGameObject(p_createdGameObject, false);
    }
}

void UndoRedoCreateGameObject::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    p_createdGameObject = nullptr;
    p_parent = nullptr;
}
