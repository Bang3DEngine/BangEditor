#ifndef RIWMATERIAL_H
#define RIWMATERIAL_H

#include "Bang/Path.h"
#include "Bang/Material.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/RIWResource.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UISlider;
FORWARD class UICheckBox;
FORWARD class UIComboBox;
FORWARD class UIInputNumber;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputColor;
FORWARD class UIInputVector;
FORWARD class PreviewViewer;
FORWARD class UIInputTexture;
FORWARD class UIInputFileWithPreview;

class RIWMaterial : public RIWResource<Material>
{
    GAMEOBJECT_EDITOR(RIWMaterial);

public:
    // InspectorWidget
    void Init() override;

private:
    UIComboBox *p_renderPassInput = nullptr;
    UIInputColor *p_albedoColorInput = nullptr;
    UIInputTexture *p_albedoTextureInput = nullptr;
    UIInputVector *p_albedoUvMultiplyInput = nullptr;
    UIInputVector *p_albedoUvOffsetInput   = nullptr;
    UIInputTexture *p_roughnessTextureInput = nullptr;
    UIInputTexture *p_metalnessTextureInput = nullptr;
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
    UIInputFileWithPreview *p_vertexShaderInput = nullptr;
    UIInputFileWithPreview *p_fragmentShaderInput = nullptr;
    PreviewViewer *p_materialPreviewViewer = nullptr;

	RIWMaterial();
    virtual ~RIWMaterial();

    Material *GetMaterial() const;

    // RIWResource
    void UpdateInputsFromResource() override;
    Texture2D* GetIconTexture() const override;

    // RIWResource
    void OnValueChangedRIWResource(EventEmitter<IEventsValueChanged> *object) override;

    friend class ResourceInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // RIWMATERIAL_H

