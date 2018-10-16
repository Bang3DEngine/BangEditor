#include "BangEditor/UndoRedoSerializableChange.h"

#include "Bang/Component.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/Path.h"
#include "Bang/Resource.h"
#include "Bang/Serializable.h"
#include "Bang/String.h"
#include "BangEditor/Editor.h"
#include "BangEditor/Explorer.h"
#include "BangEditor/Inspector.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoSerializableChange::UndoRedoSerializableChange(Serializable *serializable,
                                                       const MetaNode &metaBefore,
                                                       const MetaNode &metaAfter)
{
    p_serializable = serializable;
    SetMetaBefore(metaBefore);
    SetMetaAfter(metaAfter);

    if (auto *destroyable = DCAST<EventEmitter<IEventsDestroy>*>(p_serializable))
    {
        destroyable->EventEmitter<IEventsDestroy>::RegisterListener(this);
    }
}

UndoRedoSerializableChange::~UndoRedoSerializableChange()
{
}

void UndoRedoSerializableChange::SetMetaBefore(const MetaNode &metaBefore)
{
    m_metaBefore = metaBefore;
}

void UndoRedoSerializableChange::SetMetaAfter(const MetaNode &metaAfter)
{
    m_metaAfter = metaAfter;
}

void UndoRedoSerializableChange::Undo()
{
    if (p_serializable)
    {
        p_serializable->ImportMeta(m_metaBefore);
        SelectSerializableOrShowInInspectorIfPossible();
    }
}

void UndoRedoSerializableChange::Redo()
{
    if (p_serializable)
    {
        p_serializable->ImportMeta(m_metaAfter);
        SelectSerializableOrShowInInspectorIfPossible();
    }
}

bool UndoRedoSerializableChange::IsRedundant() const
{
    return (m_metaBefore.ToString() == m_metaAfter.ToString());
}

void UndoRedoSerializableChange::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    BANG_UNUSED(object);
    p_serializable = nullptr;
}

void UndoRedoSerializableChange::SelectSerializableOrShowInInspectorIfPossible() const
{
    GameObject *go = DCAST<GameObject*>(p_serializable);
    if (!go)
    {
        Component *comp = DCAST<Component*>(p_serializable);
        if (comp)
        {
            go = comp->GetGameObject();
        }
    }

    if (go)
    {
        Editor::SelectGameObject(go, false);
    }
    else
    {
        bool keepTrying = true;
        if (Resource *res = DCAST<Resource*>(p_serializable))
        {
            Path path = res->GetResourceFilepath();
            if (path.IsFile())
            {
                Explorer::GetInstance()->SelectPath(path, false);
                keepTrying = false;
            }
        }

        if (keepTrying)
        {
            Inspector::GetActive()->ShowSerializable(p_serializable);
        }
    }
}

