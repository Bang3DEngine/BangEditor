#ifndef AIWPHYSICSMATERIAL_H
#define AIWPHYSICSMATERIAL_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/PhysicsMaterial.h"
#include "Bang/String.h"
#include "BangEditor/AIWAsset.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
class UIComboBox;
class UIInputNumber;
class UISlider;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class AIWPhysicsMaterial : public AIWAsset<PhysicsMaterial>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(AIWPhysicsMaterial);

public:
    // InspectorWidget
    void Init() override;

private:
    AH<PhysicsMaterial> m_physicsMaterialAH;

    UISlider *p_staticFrictionInput = nullptr;
    UISlider *p_dynamicFrictionInput = nullptr;
    UISlider *p_restitutionInput = nullptr;
    UIComboBox *p_frictionCombineModeInput = nullptr;
    UIComboBox *p_restitutionCombineModeInput = nullptr;

    AIWPhysicsMaterial();
    virtual ~AIWPhysicsMaterial() override;

    PhysicsMaterial *GetPhysicsMaterial() const;

    // AIWAsset
    void UpdateInputsFromAsset() override;
    Texture2D *GetIconTexture() const override;

    // AIWAsset
    void OnValueChangedAIWAsset(
        EventEmitter<IEventsValueChanged> *object) override;

    friend class AssetInspectorWidgetFactory;
};
}

#endif  // AIWPHYSICSMATERIAL_H
