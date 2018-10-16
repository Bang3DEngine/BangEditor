#ifndef UNDOREDOSERIALIZABLECHANGE_H
#define UNDOREDOSERIALIZABLECHANGE_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/MetaNode.h"
#include "Bang/Serializable.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class IEventsDestroy;
FORWARD   class Serializable;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoSerializableChange : public UndoRedoAction,
                                   public EventListener<IEventsDestroy>
{
public:
    UndoRedoSerializableChange(Serializable *serializable,
                               const MetaNode &metaBefore,
                               const MetaNode &metaAfter);
    virtual ~UndoRedoSerializableChange() override;

    void SetMetaBefore(const MetaNode &metaBefore);
    void SetMetaAfter(const MetaNode &metaAfter);

    // UndoRedoAction
    void Undo() override;
    void Redo() override;
    bool IsRedundant() const override;

private:
    Serializable *p_serializable = nullptr;
    MetaNode m_metaBefore;
    MetaNode m_metaAfter;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

    void SelectSerializableOrShowInInspectorIfPossible() const;
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOSERIALIZABLECHANGE_H

