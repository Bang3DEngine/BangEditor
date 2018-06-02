#ifndef FILEINSPECTORWIDGET_H
#define FILEINSPECTORWIDGET_H

#include "Bang/Path.h"
#include "Bang/XMLNode.h"
#include "Bang/EventEmitter.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/InspectorWidget.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UndoRedoFileChange;
FORWARD class UndoRedoSerializableChange;

class FileInspectorWidget : public InspectorWidget
{
protected:
    FileInspectorWidget() = default;
    virtual ~FileInspectorWidget() = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void Update() override;

    virtual void UpdateFromFileWhenChanged() = 0;

    void SetPath(const Path &path);
    Path GetPath() const;

protected:
    void PushBeginUndoRedoSerializableChange(Serializable *serializable);
    void PushEndUndoRedoSerializableChange();

    void PushBeginUndoRedoFileChange(const Path &path);
    void PushEndUndoRedoFileChange();

    void PushEndUndoRedoSerializableChangeAndFileChangeTogether();

private:
    Path m_path = Path::Empty;

    XMLNode m_undoSerializableXMLBefore;
    Serializable *p_undoSerializable = nullptr;
    UndoRedoFileChange *p_undoRedoFileChange = nullptr;

    UndoRedoFileChange* GetEndUndoRedoFileChange();
    UndoRedoSerializableChange* GetEndUndoRedoSerializableChange();

    friend class FileInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FILEINSPECTORWIDGET_H

