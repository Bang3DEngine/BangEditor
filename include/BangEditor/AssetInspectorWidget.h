#ifndef FILEINSPECTORWIDGET_H
#define FILEINSPECTORWIDGET_H

#include "Bang/BangDefines.h"
#include "BangMath/Color.h"
#include "Bang/Path.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SerializableInspectorWidget.h"

namespace Bang
{
class Texture2D;
}

using namespace Bang;
namespace BangEditor
{
class AssetInspectorWidget : public SerializableInspectorWidget
{
protected:
    AssetInspectorWidget() = default;
    virtual ~AssetInspectorWidget() = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void Update() override;

    void SetPath(const Path &path);
    Path GetPath() const;

    virtual void UpdateFromFileWhenChanged();
    virtual Texture2D *GetIconTexture() const;
    virtual Color GetIconTint() const;
    void UpdateIcon();

private:
    Path m_path = Path::Empty();

    friend class AssetInspectorWidgetFactory;
};
}  // namespace BangEditor

#endif  // FILEINSPECTORWIDGET_H
