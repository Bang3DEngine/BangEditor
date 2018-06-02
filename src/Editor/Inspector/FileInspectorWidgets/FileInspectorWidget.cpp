#include "BangEditor/FileInspectorWidget.h"

#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoFileChange.h"
#include "BangEditor/UndoRedoSerializableChange.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void FileInspectorWidget::InitInnerWidgets()
{
    InspectorWidget::InitInnerWidgets();
}

void FileInspectorWidget::Update()
{
    InspectorWidget::Update();
}

void FileInspectorWidget::SetPath(const Path &path)
{
    if (path != GetPath())
    {
        m_path = path;
        UpdateFromFileWhenChanged();
    }
}

Path FileInspectorWidget::GetPath() const
{
    return m_path;
}

void FileInspectorWidget::PushBeginUndoRedoSerializableChange(
                                                    Serializable *serializable)
{
    p_undoSerializable = serializable;
    m_undoSerializableXMLBefore = serializable->GetXMLInfo();
}

void FileInspectorWidget::PushEndUndoRedoSerializableChange()
{
    ASSERT(p_undoSerializable);
    UndoRedoManager::PushAction( GetEndUndoRedoSerializableChange() );
}

void FileInspectorWidget::PushBeginUndoRedoFileChange(const Path &path)
{
    ASSERT(!p_undoRedoFileChange);
    p_undoRedoFileChange = new UndoRedoFileChange(path);
    p_undoRedoFileChange->ReadBefore();
}

void FileInspectorWidget::PushEndUndoRedoFileChange()
{
    UndoRedoManager::PushAction( GetEndUndoRedoFileChange() );
}

void FileInspectorWidget::PushEndUndoRedoSerializableChangeAndFileChangeTogether()
{
    UndoRedoManager::PushActionsInSameStep( { GetEndUndoRedoSerializableChange(),
                                              GetEndUndoRedoFileChange()});
}

UndoRedoFileChange *FileInspectorWidget::GetEndUndoRedoFileChange()
{
    ASSERT(p_undoRedoFileChange);
    p_undoRedoFileChange->ReadAfter();

    UndoRedoFileChange *undoRedo = p_undoRedoFileChange;
    p_undoRedoFileChange = nullptr;

    return undoRedo;
}

UndoRedoSerializableChange *FileInspectorWidget::GetEndUndoRedoSerializableChange()
{
    ASSERT(p_undoSerializable);
    return new UndoRedoSerializableChange(p_undoSerializable,
                                          m_undoSerializableXMLBefore,
                                          p_undoSerializable->GetXMLInfo());
}
