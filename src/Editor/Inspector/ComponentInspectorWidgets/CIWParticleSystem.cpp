#include "BangEditor/CIWParticleSystem.h"

#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/Assets.h"
#include "Bang/Assets.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "BangMath/Math.h"
#include "Bang/Mesh.h"
#include "Bang/Particle.h"
#include "Bang/ParticleSystem.h"
#include "Bang/Path.h"
#include "Bang/Texture2D.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UISlider.h"
#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputComplexRandom.h"
#include "BangEditor/UIInputFile.h"
#include "BangEditor/UIInputVector.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

void CIWParticleSystem::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWParticleSystem");
    SetTitle("Particle System");

    p_meshInputFile = new UIInputFile();
    p_meshInputFile->SetExtensions({Extensions::GetMeshExtension()});
    p_meshInputFile->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_lifeTimeInput = new UIInputComplexRandom();
    p_lifeTimeInput->SetRangeMinValue(0);
    p_lifeTimeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_startTimeInput = new UIInputComplexRandom();
    p_startTimeInput->SetRangeMinValue(0);
    p_startTimeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_startSizeInput = new UIInputComplexRandom();
    p_startSizeInput->SetRangeMinValue(0);
    p_startSizeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_billboardInput = GameObjectFactory::CreateUICheckBox();
    p_billboardInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_textureInput = new UIInputFile();
    p_textureInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_sheetSizeInput = new UIInputVector();
    for (int i = 0; i < 2; ++i)
    {
        p_sheetSizeInput->GetInputNumbers()[i]->SetDecimalPlaces(0);
    }
    p_sheetSizeInput->SetSize(2);
    p_sheetSizeInput->SetMinValue(Vector4::One());
    p_sheetSizeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_animationSpeedInput = GameObjectFactory::CreateUIInputNumber();
    p_animationSpeedInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_particleRenderModeInput = GameObjectFactory::CreateUIComboBox();
    p_particleRenderModeInput->AddItem(
        "Additive", SCAST<int>(ParticleRenderMode::ADDITIVE));
    p_particleRenderModeInput->AddItem("Mesh",
                                       SCAST<int>(ParticleRenderMode::MESH));
    p_particleRenderModeInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_startColorInput = new UIInputColor();
    p_startColorInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_endColorInput = new UIInputColor();
    p_endColorInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_numParticlesInput = GameObjectFactory::CreateUIInputNumber();
    p_numParticlesInput->SetDecimalPlaces(0);
    p_numParticlesInput->SetMinValue(0);
    p_numParticlesInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_bouncinessInput = GameObjectFactory::CreateUISlider();
    p_bouncinessInput->SetMinMaxValues(0.0f, 1.0f);
    p_bouncinessInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_generationShapeInput = GameObjectFactory::CreateUIComboBox();
    p_generationShapeInput->AddItem("Box",
                                    SCAST<int>(ParticleGenerationShape::BOX));
    p_generationShapeInput->AddItem("Cone",
                                    SCAST<int>(ParticleGenerationShape::CONE));
    p_generationShapeInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_generationShapeBoxSizeInput = new UIInputVector();
    p_generationShapeBoxSizeInput->SetSize(3);
    p_generationShapeBoxSizeInput->SetMinValue(Vector4::Zero());
    p_generationShapeBoxSizeInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_generationShapeConeFOVInput = GameObjectFactory::CreateUIInputNumber();
    p_generationShapeConeFOVInput->SetMinMaxValues(0.0f, 180.0f);
    p_generationShapeConeFOVInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_computeCollisionsInput = GameObjectFactory::CreateUICheckBox();
    p_computeCollisionsInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_physicsStepModeInput = GameObjectFactory::CreateUIComboBox();
    p_physicsStepModeInput->AddItem(
        "Euler", SCAST<int>(Particle::PhysicsStepMode::EULER));
    p_physicsStepModeInput->AddItem(
        "Euler-Semi", SCAST<int>(Particle::PhysicsStepMode::EULER_SEMI));
    p_physicsStepModeInput->AddItem(
        "Verlet", SCAST<int>(Particle::PhysicsStepMode::VERLET));
    p_physicsStepModeInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_gravityMultiplierInput = GameObjectFactory::CreateUIInputNumber();
    p_gravityMultiplierInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_initialVelocityMultiplier = GameObjectFactory::CreateUIInputNumber();
    p_initialVelocityMultiplier
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Life time", p_lifeTimeInput);
    AddWidget("Start time", p_startTimeInput);
    AddWidget("Num. Particles", p_numParticlesInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Texture", p_textureInput);
    AddWidget("Sheet size", p_sheetSizeInput);
    AddWidget("Animation speed", p_animationSpeedInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Mesh", p_meshInputFile);
    AddWidget("Billboard", p_billboardInput->GetGameObject());
    AddWidget("Render Mode", p_particleRenderModeInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Start size", p_startSizeInput);
    AddWidget("Start color", p_startColorInput);
    AddWidget("End color", p_endColorInput);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Generation shape", p_generationShapeInput->GetGameObject());
    AddWidget("Box size", p_generationShapeBoxSizeInput);
    AddWidget("Cone FOV", p_generationShapeConeFOVInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Compute collisions", p_computeCollisionsInput->GetGameObject());
    AddWidget("Bounciness", p_bouncinessInput->GetGameObject());
    AddWidget("Step mode", p_physicsStepModeInput->GetGameObject());
    AddWidget("Gravity multiplier", p_gravityMultiplierInput->GetGameObject());
    AddWidget("Init vel. multiplier",
              p_initialVelocityMultiplier->GetGameObject());
    SetLabelsWidth(100);

    EnableOnlyNeededWidgets();
}

void CIWParticleSystem::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    Mesh *mesh = GetParticleSystem()->GetMesh();
    Path meshPath = mesh ? mesh->GetAssetFilepath() : Path::Empty();
    p_meshInputFile->SetPath(meshPath);

    if (!p_lifeTimeInput->HasFocus())
    {
        p_lifeTimeInput->Set(GetParticleSystem()->GetLifeTime());
    }

    if (!p_startTimeInput->HasFocus())
    {
        p_startTimeInput->Set(GetParticleSystem()->GetStartTime());
    }

    if (!p_startSizeInput->HasFocus())
    {
        p_startSizeInput->Set(GetParticleSystem()->GetStartSize());
    }

    p_billboardInput->SetChecked(GetParticleSystem()->GetBillboard());
    p_computeCollisionsInput->SetChecked(
        GetParticleSystem()->GetComputeCollisions());
    p_textureInput->SetPath(
        GetParticleSystem()->GetTexture()
            ? GetParticleSystem()->GetTexture()->GetAssetFilepath()
            : Path::Empty());

    if (!p_sheetSizeInput->HasFocus())
    {
        p_sheetSizeInput->Set(
            Vector2(GetParticleSystem()->GetAnimationSheetSize()));
    }

    if (!p_animationSpeedInput->HasFocus())
    {
        p_animationSpeedInput->SetValue(
            GetParticleSystem()->GetAnimationSpeed());
    }

    if (!p_physicsStepModeInput->HasFocus())
    {
        p_physicsStepModeInput->SetSelectionByValue(
            SCAST<int>(GetParticleSystem()->GetPhysicsStepMode()));
    }

    if (!p_particleRenderModeInput->HasFocus())
    {
        p_particleRenderModeInput->SetSelectionByValue(
            SCAST<int>(GetParticleSystem()->GetParticleRenderMode()));
    }

    if (!p_startColorInput->HasFocus())
    {
        p_startColorInput->SetColor(GetParticleSystem()->GetStartColor());
    }

    if (!p_endColorInput->HasFocus())
    {
        p_endColorInput->SetColor(GetParticleSystem()->GetEndColor());
    }

    if (!p_numParticlesInput->HasFocus())
    {
        p_numParticlesInput->SetValue(
            SCAST<float>(GetParticleSystem()->GetNumParticles()));
    }

    if (!p_bouncinessInput->HasFocus())
    {
        p_bouncinessInput->SetValue(GetParticleSystem()->GetBounciness());
    }

    if (!p_generationShapeInput->HasFocus())
    {
        p_generationShapeInput->SetSelectionByValue(
            SCAST<int>(GetParticleSystem()->GetGenerationShape()));
    }

    if (!p_generationShapeBoxSizeInput->HasFocus())
    {
        p_generationShapeBoxSizeInput->Set(
            GetParticleSystem()->GetGenerationShapeBoxSize());
    }

    if (!p_generationShapeConeFOVInput->HasFocus())
    {
        p_generationShapeConeFOVInput->SetValue(Math::RadToDeg(
            GetParticleSystem()->GetGenerationShapeConeFOVRads()));
    }

    if (!p_gravityMultiplierInput->HasFocus())
    {
        p_gravityMultiplierInput->SetValue(
            GetParticleSystem()->GetGravityMultiplier());
    }

    if (!p_initialVelocityMultiplier->HasFocus())
    {
        p_initialVelocityMultiplier->SetValue(
            GetParticleSystem()->GetInitialVelocityMultiplier());
    }

    EnableOnlyNeededWidgets();
}

void CIWParticleSystem::EnableOnlyNeededWidgets()
{
    SetWidgetEnabled(GetWidgetFromLabel("Material"), false);
    // SetWidgetEnabled(p_castsShadowsCheckBox->GetGameObject(), false);
    // SetWidgetEnabled(p_receivesShadowsCheckBox->GetGameObject(), false);
    SetWidgetEnabled(GetWidgetFromLabel("Use Refl Probes"), false);

    Array<GameObject *> allWidgetsToEnableOrDisable;
    allWidgetsToEnableOrDisable.PushBack(p_generationShapeBoxSizeInput);
    allWidgetsToEnableOrDisable.PushBack(
        p_generationShapeConeFOVInput->GetGameObject());

    Array<GameObject *> keepWidgetsEnabled;
    if (GetParticleSystem())
    {
        switch (GetParticleSystem()->GetGenerationShape())
        {
            case ParticleGenerationShape::BOX:
                keepWidgetsEnabled.PushBack(p_generationShapeBoxSizeInput);
                break;

            case ParticleGenerationShape::CONE:
                keepWidgetsEnabled.PushBack(
                    p_generationShapeConeFOVInput->GetGameObject());
                break;
        }
    }

    for (GameObject *widget : allWidgetsToEnableOrDisable)
    {
        bool enabled = (keepWidgetsEnabled.Contains(widget));
        SetWidgetEnabled(widget, enabled);
    }
}

void CIWParticleSystem::OnValueChangedCIW(
    EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    if (object == p_meshInputFile)
    {
        Path meshPath = p_meshInputFile->GetPath();
        AH<Mesh> mesh = Assets::Load<Mesh>(meshPath);
        GetParticleSystem()->SetMesh(mesh.Get());
    }

    if (object == p_lifeTimeInput)
    {
        GetParticleSystem()->SetLifeTime(p_lifeTimeInput->GetComplexRandom());
    }

    if (object == p_startTimeInput)
    {
        GetParticleSystem()->SetStartTime(p_startTimeInput->GetComplexRandom());
    }

    if (object == p_textureInput)
    {
        AH<Texture2D> tex = Assets::Load<Texture2D>(p_textureInput->GetPath());
        GetParticleSystem()->SetTexture(tex.Get());
    }

    if (object == p_billboardInput)
    {
        GetParticleSystem()->SetBillboard(p_billboardInput->IsChecked());
    }

    if (object == p_bouncinessInput)
    {
        GetParticleSystem()->SetBounciness(p_bouncinessInput->GetValue());
    }

    if (object == p_animationSpeedInput)
    {
        GetParticleSystem()->SetAnimationSpeed(
            p_animationSpeedInput->GetValue());
    }

    if (object == p_sheetSizeInput)
    {
        GetParticleSystem()->SetAnimationSheetSize(
            Vector2i(p_sheetSizeInput->GetVector2()));
    }

    if (object == p_particleRenderModeInput)
    {
        GetParticleSystem()->SetParticleRenderMode(SCAST<ParticleRenderMode>(
            p_particleRenderModeInput->GetSelectedValue()));
    }

    if (object == p_startColorInput)
    {
        GetParticleSystem()->SetStartColor(p_startColorInput->GetColor());
    }

    if (object == p_endColorInput)
    {
        GetParticleSystem()->SetEndColor(p_endColorInput->GetColor());
    }

    if (object == p_numParticlesInput)
    {
        GetParticleSystem()->SetNumParticles(
            SCAST<uint>(p_numParticlesInput->GetValue()));
    }

    if (object == p_physicsStepModeInput)
    {
        GetParticleSystem()->SetPhysicsStepMode(
            SCAST<Particle::PhysicsStepMode>(
                p_physicsStepModeInput->GetSelectedValue()));
    }

    if (object == p_startSizeInput)
    {
        GetParticleSystem()->SetStartSize(p_startSizeInput->GetComplexRandom());
    }

    if (object == p_generationShapeInput)
    {
        GetParticleSystem()->SetGenerationShape(SCAST<ParticleGenerationShape>(
            p_generationShapeInput->GetSelectedValue()));
    }

    GetParticleSystem()->SetGenerationShapeBoxSize(
        p_generationShapeBoxSizeInput->GetVector3());
    GetParticleSystem()->SetGenerationShapeConeFOVRads(
        Math::DegToRad(p_generationShapeConeFOVInput->GetValue()));

    if (object == p_computeCollisionsInput)
    {
        GetParticleSystem()->SetComputeCollisions(
            p_computeCollisionsInput->IsChecked());
    }

    if (object == p_gravityMultiplierInput)
    {
        GetParticleSystem()->SetGravityMultiplier(
            p_gravityMultiplierInput->GetValue());
    }

    if (object == p_initialVelocityMultiplier)
    {
        GetParticleSystem()->SetInitialVelocityMultiplier(
            p_initialVelocityMultiplier->GetValue());
    }

    EnableOnlyNeededWidgets();
}

ParticleSystem *CIWParticleSystem::GetParticleSystem() const
{
    return SCAST<ParticleSystem *>(GetComponent());
}
