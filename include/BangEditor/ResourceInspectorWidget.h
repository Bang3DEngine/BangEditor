#ifndef FILEINSPECTORWIDGET_H
#define FILEINSPECTORWIDGET_H

#include "Bang/Path.h"

#include "BangEditor/InspectorWidget.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ResourceInspectorWidget : public InspectorWidget
{
protected:
    ResourceInspectorWidget() = default;
    virtual ~ResourceInspectorWidget() = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void Update() override;

    void SetPath(const Path &path);
    Path GetPath() const;

    virtual void UpdateFromFileWhenChanged() = 0;

private:
    Path m_path = Path::Empty;

    friend class ResourceInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FILEINSPECTORWIDGET_H

