#ifndef FIWMATERIAL_H
#define FIWMATERIAL_H

#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"

#include "BangEditor/FileInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UICheckBox;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class FIWMaterial : public FileInspectorWidget
{
    GAMEOBJECT_EDITOR(FIWMaterial);

private:
    RH<Material> p_material;

    UICheckBox *p_receivesLightingCheckBox = nullptr;

	FIWMaterial();
    virtual ~FIWMaterial();

    // FileInspectorWidget
    void OnPathChanged(const Path &path) override;

    friend class FileInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FIWMATERIAL_H

