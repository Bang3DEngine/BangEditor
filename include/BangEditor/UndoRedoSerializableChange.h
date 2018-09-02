#ifndef UNDOREDOSERIALIZABLECHANGE_H
#define UNDOREDOSERIALIZABLECHANGE_H

#include "Bang/Bang.h"
#include "Bang/XMLNode.h"
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
                               const XMLNode &xmlBefore,
                               const XMLNode &xmlAfter);
	virtual ~UndoRedoSerializableChange();

    void SetXMLBefore(const XMLNode &xmlBefore);
    void SetXMLAfter(const XMLNode &xmlAfter);

    void Undo() override;
    void Redo() override;

private:
    Serializable *p_serializable = nullptr;
    XMLNode m_xmlBefore;
    XMLNode m_xmlAfter;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

    void SelectSerializableOrShowInInspectorIfPossible() const;
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOSERIALIZABLECHANGE_H

