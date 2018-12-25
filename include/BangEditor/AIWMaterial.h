#ifndef AIWMATERIAL_H
#define AIWMATERIAL_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Material.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "BangEditor/AIWAsset.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
class UICheckBox;
class UIComboBox;
class UIInputNumber;
class UISlider;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class PreviewViewer;
class UIInputColor;
class UIInputFile;
class UIInputFile;
class UIInputVector;

class AIWMaterial : public AIWAsset<Material>
{
    GAMEOBJECT_EDITOR(AIWMaterial);

public:
    // InspectorWidget
    void Init() override;

private:
    UIInputColor *p_albedoColorInput = nullptr;
    UIInputFile *p_albedoTextureInput = nullptr;
    UIInputVector *p_albedoUvMultiplyInput = nullptr;
    UIInputVector *p_albedoUvOffsetInput = nullptr;
    UIInputFile *p_roughnessTextureInput = nullptr;
    UIInputFile *p_metalnessTextureInput = nullptr;
    UIInputFile *p_normalMapTextureInput = nullptr;
    UIInputVector *p_normalMapUvMultiplyInput = nullptr;
    UIInputVector *p_normalMapUvOffsetInput = nullptr;
    UIInputNumber *p_normalMapMultiplyFactorInput = nullptr;
    UICheckBox *p_receivesLightingCheckBox = nullptr;
    UISlider *p_metalnessSlider = nullptr;
    UISlider *p_roughnessSlider = nullptr;
    UIInputNumber *p_lineWidthInput = nullptr;
    UIComboBox *p_cullFaceInput = nullptr;
    UICheckBox *p_renderWireframe = nullptr;
    UIComboBox *p_renderPassInput = nullptr;
    UIComboBox *p_neededUniformsInput = nullptr;
    UIInputFile *p_vertexShaderInput = nullptr;
    UIInputFile *p_fragmentShaderInput = nullptr;
    PreviewViewer *p_materialPreviewViewer = nullptr;

    AIWMaterial();
    virtual ~AIWMaterial() override;

    Material *GetMaterial() const;

    // AIWAsset
    void UpdateInputsFromAsset() override;
    Texture2D *GetIconTexture() const override;

    // AIWAsset
    void OnValueChangedAIWAsset(
        EventEmitter<IEventsValueChanged> *object) override;

    friend class AssetInspectorWidgetFactory;
};
}

#endif  // AIWMATERIAL_H
