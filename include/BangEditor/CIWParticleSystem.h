#ifndef CIWPARTICLESYSTEM_H
#define CIWPARTICLESYSTEM_H

#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class IEventsValueChanged;
class UICheckBox;
class ParticleSystem;
class UIComboBox;
class UIInputNumber;
class UISlider;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputColor;
class UIInputComplexRandom;
class UIInputFile;
class UIInputVector;

class CIWParticleSystem : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(CIWParticleSystem);

public:
    CIWParticleSystem() = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    UIInputFile *p_meshInputFile = nullptr;

    UIInputComplexRandom *p_lifeTimeInput = nullptr;
    UIInputComplexRandom *p_startTimeInput = nullptr;
    UIInputComplexRandom *p_startSizeInput = nullptr;
    UICheckBox *p_billboardInput = nullptr;
    UIInputFile *p_textureInput = nullptr;
    UIInputVector *p_sheetSizeInput = nullptr;
    UISlider *p_bouncinessInput = nullptr;
    UIInputNumber *p_animationSpeedInput = nullptr;
    UIComboBox *p_particleRenderModeInput = nullptr;
    UIInputColor *p_startColorInput = nullptr;
    UIInputColor *p_endColorInput = nullptr;
    UIInputNumber *p_numParticlesInput = nullptr;
    UIComboBox *p_physicsStepModeInput = nullptr;

    UIComboBox *p_generationShapeInput = nullptr;
    UIInputVector *p_generationShapeBoxSizeInput = nullptr;
    UIInputNumber *p_generationShapeConeFOVInput = nullptr;

    UICheckBox *p_computeCollisionsInput = nullptr;
    UIInputNumber *p_gravityMultiplierInput = nullptr;
    UIInputNumber *p_initialVelocityMultiplier = nullptr;

    virtual ~CIWParticleSystem() override = default;

    void EnableOnlyNeededWidgets();
    ParticleSystem *GetParticleSystem() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;
};
}

#endif  // CIWPARTICLESYSTEM_H
