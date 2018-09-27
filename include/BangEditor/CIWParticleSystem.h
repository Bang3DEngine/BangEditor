#ifndef CIWPARTICLESYSTEM_H
#define CIWPARTICLESYSTEM_H

#include "BangEditor/CIWRenderer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIComboBox;
FORWARD class UIInputNumber;
FORWARD class ParticleSystem;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputVector;

FORWARD class UIInputFileWithPreview;

class CIWParticleSystem : public CIWRenderer
{
    GAMEOBJECT_EDITOR(CIWParticleSystem);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    UIInputFileWithPreview *p_meshInputFile = nullptr;
    UIInputNumber *p_num = nullptr;
    UIInputNumber *p_lifetimeInput = nullptr;
    UIInputNumber *p_numParticlesInput = nullptr;
    UIComboBox *p_generationShapeInput = nullptr;
    UIInputVector *p_generationShapeBoxSizeInput = nullptr;
    UIInputNumber *p_gravityMultiplierInput = nullptr;
    UIInputNumber *p_initialVelocityMultiplier = nullptr;

    CIWParticleSystem() = default;
    virtual ~CIWParticleSystem() = default;

    ParticleSystem *GetParticleSystem() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWPARTICLESYSTEM_H

