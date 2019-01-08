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
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(AIWMaterial);

public:
    // InspectorWidget
    void Init() override;

private:
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
