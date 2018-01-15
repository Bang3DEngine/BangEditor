#ifndef FILEINSPECTORWIDGET_H
#define FILEINSPECTORWIDGET_H

#include "Bang/Path.h"

#include "BangEditor/InspectorWidget.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

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

private:
    Path m_path = Path::Empty;

    friend class FileInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FILEINSPECTORWIDGET_H

