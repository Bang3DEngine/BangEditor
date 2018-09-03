#ifndef UNDOREDOSERIALIZABLECHANGE_H
#define UNDOREDOSERIALIZABLECHANGE_H

#include "Bang/Bang.h"
#include "Bang/MetaNode.h"
#include "Bang/Serializable.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/IEventsDestroy.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoSerializableChange : public UndoRedoAction,
                                   public EventListener<IEventsDestroy>
{
public:
    UndoRedoSerializableChange(Serializable *serializable,
                               const MetaNode &metaBefore,
                               const MetaNode &metaAfter);
	virtual ~UndoRedoSerializableChange();

    void SetMetaBefore(const MetaNode &metaBefore);
    void SetMetaAfter(const MetaNode &metaAfter);

    void Undo() override;
    void Redo() override;

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

