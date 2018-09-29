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

FORWARD class UIInputColor;
FORWARD class UIInputVector;
FORWARD class UIInputTexture;
FORWARD class UIInputComplexRandom;
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

    UIInputComplexRandom *p_lifeTimeInput = nullptr;
    UIInputComplexRandom *p_startTimeInput = nullptr;
    UIInputComplexRandom *p_startSizeInput = nullptr;
    UICheckBox *p_billboardInput = nullptr;
    UIInputTexture *p_textureInput = nullptr;
    UIInputVector *p_sheetSizeInput = nullptr;
    UIInputNumber *p_animationSpeedInput = nullptr;
    UIComboBox *p_particleRenderModeInput = nullptr;
    UIInputColor *p_startColorInput = nullptr;
    UIInputColor *p_endColorInput = nullptr;
    UIInputNumber *p_numParticlesInput = nullptr;

    UIComboBox *p_generationShapeInput = nullptr;
    UIInputVector *p_generationShapeBoxSizeInput = nullptr;
    UIInputNumber *p_generationShapeConeFOVInput = nullptr;

    UICheckBox *p_computeCollisionsInput = nullptr;
    UIInputNumber *p_gravityMultiplierInput = nullptr;
    UIInputNumber *p_initialVelocityMultiplier = nullptr;

    CIWParticleSystem() = default;
    virtual ~CIWParticleSystem() = default;

    void EnableOnlyNeededWidgets();
    ParticleSystem *GetParticleSystem() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWPARTICLESYSTEM_H

