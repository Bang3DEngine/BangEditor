#ifndef RIWPHYSICSMATERIAL_H
#define RIWPHYSICSMATERIAL_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/PhysicsMaterial.h"
#include "Bang/ResourceHandle.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/RIWResource.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class IEventsValueChanged;
FORWARD   class Texture2D;
FORWARD   class UIComboBox;
FORWARD   class UIInputNumber;
FORWARD   class UISlider;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class RIWPhysicsMaterial : public RIWResource<PhysicsMaterial>
{
    GAMEOBJECT_EDITOR(RIWPhysicsMaterial);

public:
    // InspectorWidget
    void Init() override;

private:
    RH<PhysicsMaterial> m_physicsMaterialRH;

    UISlider *p_staticFrictionInput           = nullptr;
    UISlider *p_dynamicFrictionInput          = nullptr;
    UISlider *p_restitutionInput              = nullptr;
    UIComboBox *p_frictionCombineModeInput    = nullptr;
    UIComboBox *p_restitutionCombineModeInput = nullptr;

    RIWPhysicsMaterial();
    virtual ~RIWPhysicsMaterial() override;

    PhysicsMaterial *GetPhysicsMaterial() const;

    // RIWResource
    void UpdateInputsFromResource() override;
    Texture2D *GetIconTexture() const override;

    // RIWResource
    void OnValueChangedRIWResource(EventEmitter<IEventsValueChanged> *object) override;

    friend class ResourceInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // RIWPHYSICSMATERIAL_H

