#ifndef UNDOREDOSERIALIZABLECHANGE_H
#define UNDOREDOSERIALIZABLECHANGE_H

#include "Bang/Bang.h"
#include "Bang/XMLNode.h"
#include "Bang/Serializable.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoSerializableChange : public UndoRedoAction
{
public:
    UndoRedoSerializableChange(Serializable *serializable,
                               const XMLNode &xmlBefore,
                               const XMLNode &xmlAfter);
	virtual ~UndoRedoSerializableChange();

    void Undo() override;
    void Redo() override;

private:
    Serializable *p_serializable = nullptr;
    XMLNode m_xmlBefore;
    XMLNode m_xmlAfter;

    void SelectSerializableOrShowInInspectorIfPossible() const;
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOSERIALIZABLECHANGE_H

