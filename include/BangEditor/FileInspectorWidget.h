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
    virtual void Init() override;

    void SetPath(const Path &path);
    Path GetPath() const;

    virtual void OnPathChanged(const Path &path) = 0;

private:
    Path m_path = Path::Empty;

    friend class FileInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FILEINSPECTORWIDGET_H

