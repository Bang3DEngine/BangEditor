#ifndef FIWMATERIAL_H
#define FIWMATERIAL_H

#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IValueChangedListener.h"

#include "BangEditor/FileInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UISlider;
FORWARD class UICheckBox;
FORWARD class UIComboBox;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;
FORWARD class UIInputColor;
FORWARD class UIInputVector;

class FIWMaterial : public FileInspectorWidget,
                    public IValueChangedListener
{
    GAMEOBJECT_EDITOR(FIWMaterial);

public:
    // InspectorWidget
    void Init() override;

private:
    RH<Material> p_material;

    UIComboBox *p_renderPassInput = nullptr;
    UIInputFile *p_texturePathInput = nullptr;
    UIInputVector *p_uvMultiplyInput = nullptr;
    UIInputColor *p_diffuseColorInput = nullptr;
    UICheckBox *p_receivesLightingCheckBox = nullptr;
    UISlider *p_shininessSlider = nullptr;
    UIInputFile *p_vertexShaderInput = nullptr;
    UIInputFile *p_fragmentShaderInput = nullptr;

	FIWMaterial();
    virtual ~FIWMaterial();

    Material *GetMaterial() const;

    // FileInspectorWidget
    void UpdateFromFileWhenChanged() override;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;

    friend class FileInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FIWMATERIAL_H

