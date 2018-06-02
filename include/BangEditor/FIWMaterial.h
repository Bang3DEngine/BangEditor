#ifndef FIWMATERIAL_H
#define FIWMATERIAL_H

#include "Bang/Path.h"
#include "Bang/Material.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/FIWResource.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UISlider;
FORWARD class UICheckBox;
FORWARD class UIComboBox;
FORWARD class UIInputNumber;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFile;
FORWARD class UIInputColor;
FORWARD class UIInputVector;
FORWARD class UIInputTexture;

class FIWMaterial : public FIWResource<Material>
{
    GAMEOBJECT_EDITOR(FIWMaterial);

public:
    // InspectorWidget
    void Init() override;

private:
    RH<Material> m_materialRH;

    UIComboBox *p_renderPassInput = nullptr;
    UIInputColor *p_albedoColorInput = nullptr;
    UIInputTexture *p_albedoTextureInput = nullptr;
    UIInputVector *p_albedoUvMultiplyInput = nullptr;
    UIInputVector *p_albedoUvOffsetInput   = nullptr;
    UIInputTexture *p_normalMapTextureInput = nullptr;
    UIInputVector *p_normalMapUvMultiplyInput = nullptr;
    UIInputVector *p_normalMapUvOffsetInput   = nullptr;
    UIInputNumber *p_normalMapMultiplyFactorInput = nullptr;
    UICheckBox *p_receivesLightingCheckBox = nullptr;
    UISlider *p_metalnessSlider = nullptr;
    UISlider *p_roughnessSlider = nullptr;
    UIInputNumber *p_lineWidthInput = nullptr;
    UIComboBox *p_cullFaceInput = nullptr;
    UICheckBox *p_renderWireframe = nullptr;
    UIInputFile *p_vertexShaderInput = nullptr;
    UIInputFile *p_fragmentShaderInput = nullptr;
    UIImageRenderer *p_materialPreviewImg = nullptr;

	FIWMaterial();
    virtual ~FIWMaterial();

    Material *GetMaterial() const;

    // FIWResource
    void UpdateInputsFromResource() override;

    // FIWResource
    void OnValueChangedFIWResource(EventEmitter<IEventsValueChanged> *object) override;

    friend class FileInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FIWMATERIAL_H

