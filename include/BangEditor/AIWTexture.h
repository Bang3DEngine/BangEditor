#ifndef AIWTEXTURE_H
#define AIWTEXTURE_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/Texture2D.h"
#include "BangEditor/AIWAsset.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
class UIAspectRatioFitter;
class UICheckBox;
class UIComboBox;
class UIImageRenderer;
class UIInputText;
class UISlider;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class AIWTexture : public AIWAsset<Texture2D>
{
    GAMEOBJECT_EDITOR(AIWTexture);

public:
    // InspectorWidget
    void Init() override;

private:
    UIComboBox *p_filterModeComboBox = nullptr;
    UIComboBox *p_wrapModeComboBox = nullptr;
    UISlider *p_alphaCutoffInput = nullptr;
    UICheckBox *p_SRGBCheckBoxInput = nullptr;

    UIInputText *p_textureWidth = nullptr;
    UIInputText *p_textureHeight = nullptr;
    UIImageRenderer *p_textureImageRend = nullptr;
    UIAspectRatioFitter *p_imageAspectRatioFitter = nullptr;

    AIWTexture();
    virtual ~AIWTexture() override;

    Texture2D *GetTexture() const;

    // AIWAsset
    void UpdateInputsFromAsset() override;
    Texture2D *GetIconTexture() const override;

    // AIWAsset
    void OnValueChangedAIWAsset(
        EventEmitter<IEventsValueChanged> *object) override;

    friend class AssetInspectorWidgetFactory;
};
}

#endif  // AIWTEXTURE_H
