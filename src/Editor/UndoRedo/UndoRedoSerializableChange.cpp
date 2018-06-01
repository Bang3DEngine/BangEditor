#include "BangEditor/UndoRedoSerializableChange.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoSerializableChange::UndoRedoSerializableChange(Serializable *serializable,
                                                       const XMLNode &xmlBefore,
                                                       const XMLNode &xmlAfter)
{
    p_serializable = serializable;
    m_xmlBefore = xmlBefore;
    m_xmlAfter = xmlAfter;
}

UndoRedoSerializableChange::~UndoRedoSerializableChange()
{
}

void UndoRedoSerializableChange::Undo()
{
    p_serializable->ImportXML(m_xmlBefore);
}

void UndoRedoSerializableChange::Redo()
{
    p_serializable->ImportXML(m_xmlAfter);
}

