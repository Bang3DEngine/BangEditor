#ifndef RIWPHYSICSMATERIAL_H
#define RIWPHYSICSMATERIAL_H

#include "Bang/Bang.h"
#include "Bang/PhysicsMaterial.h"

#include "BangEditor/RIWResource.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UISlider;
FORWARD class UIComboBox;
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
    virtual ~RIWPhysicsMaterial();

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

