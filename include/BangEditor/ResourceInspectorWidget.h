#ifndef FILEINSPECTORWIDGET_H
#define FILEINSPECTORWIDGET_H

#include "Bang/BangDefines.h"
#include "Bang/Color.h"
#include "Bang/Path.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/InspectorWidget.h"

namespace Bang
{
class Texture2D;
}

using namespace Bang;
namespace BangEditor
{
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
    virtual Texture2D *GetIconTexture() const;
    virtual Color GetIconTint() const;
    void UpdateIcon();

private:
    Path m_path = Path::Empty();

    friend class ResourceInspectorWidgetFactory;
};
}  // namespace BangEditor

#endif  // FILEINSPECTORWIDGET_H
