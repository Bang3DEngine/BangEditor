#ifndef FIWPHYSICSMATERIAL_H
#define FIWPHYSICSMATERIAL_H

#include "Bang/Bang.h"
#include "Bang/PhysicsMaterial.h"

#include "BangEditor/FIWResource.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UISlider;
FORWARD class UIComboBox;
FORWARD NAMESPACE_BANG_END


USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class FIWPhysicsMaterial : public FIWResource<PhysicsMaterial>
{
    GAMEOBJECT_EDITOR(FIWPhysicsMaterial);

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

    FIWPhysicsMaterial();
    virtual ~FIWPhysicsMaterial();

    PhysicsMaterial *GetPhysicsMaterial() const;

    // FIWResource
    void UpdateInputsFromResource() override;

    // FIWResource
    void OnValueChangedFIWResource(EventEmitter<IEventsValueChanged> *object) override;

    friend class FileInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FIWPHYSICSMATERIAL_H

