#include "BangEditor/CIWParticleSystem.h"

#include "Bang/Mesh.h"
#include "Bang/UILabel.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIInputText.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UITextRenderer.h"
#include "Bang/ParticleSystem.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputColor.h"
#include "BangEditor/UIInputVector.h"
#include "BangEditor/UIInputComplexRandom.h"
#include "BangEditor/UIInputFileWithPreview.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWParticleSystem::InitInnerWidgets()
{
    CIWRenderer::InitInnerWidgets();

    SetName("CIWParticleSystem");
    SetTitle("Particle System");

    p_meshInputFile = GameObject::Create<UIInputFileWithPreview>();
    p_meshInputFile->SetExtensions( { Extensions::GetMeshExtension() } );
    p_meshInputFile->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_lifeTimeInput = GameObject::Create<UIInputComplexRandom>();
    p_lifeTimeInput->SetRangeMinValue(0);
    p_lifeTimeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_startTimeInput = GameObject::Create<UIInputComplexRandom>();
    p_startTimeInput->SetRangeMinValue(0);
    p_startTimeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_startSizeInput = GameObject::Create<UIInputComplexRandom>();
    p_startSizeInput->SetRangeMinValue(0);
    p_startSizeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_billboardInput = GameObjectFactory::CreateUICheckBox();
    p_billboardInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_startColorInput = GameObject::Create<UIInputColor>();
    p_startColorInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_endColorInput = GameObject::Create<UIInputColor>();
    p_endColorInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);


    p_numParticlesInput = GameObjectFactory::CreateUIInputNumber();
    p_numParticlesInput->SetDecimalPlaces(0);
    p_numParticlesInput->SetMinValue(0);
    p_numParticlesInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_generationShapeInput = GameObjectFactory::CreateUIComboBox();
    p_generationShapeInput->AddItem("Box",  SCAST<int>(ParticleGenerationShape::BOX));
    p_generationShapeInput->AddItem("Cone", SCAST<int>(ParticleGenerationShape::CONE));
    p_generationShapeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_generationShapeBoxSizeInput = GameObject::Create<UIInputVector>();
    p_generationShapeBoxSizeInput->SetSize(3);
    p_generationShapeBoxSizeInput->SetMinValue(Vector4::Zero);
    p_generationShapeBoxSizeInput->EventEmitter<IEventsValueChanged>::
                                   RegisterListener(this);

    p_generationShapeConeFOVInput = GameObjectFactory::CreateUIInputNumber();
    p_generationShapeConeFOVInput->SetMinMaxValues(0.0f, 180.0f);
    p_generationShapeConeFOVInput->EventEmitter<IEventsValueChanged>::
                                   RegisterListener(this);

    p_gravityMultiplierInput = GameObjectFactory::CreateUIInputNumber();
    p_gravityMultiplierInput->EventEmitter<IEventsValueChanged>::
                              RegisterListener(this);

    p_initialVelocityMultiplier = GameObjectFactory::CreateUIInputNumber();
    p_initialVelocityMultiplier->EventEmitter<IEventsValueChanged>::
                                 RegisterListener(this);

    AddWidget("Mesh", p_meshInputFile);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Life time",      p_lifeTimeInput);
    AddWidget("Start time",     p_startTimeInput);
    AddWidget("Num. Particles", p_numParticlesInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Billboard", p_billboardInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Start size",  p_startSizeInput);
    AddWidget("Start color", p_startColorInput);
    AddWidget("End color",   p_endColorInput);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Generation shape", p_generationShapeInput->GetGameObject());
    AddWidget("Box size", p_generationShapeBoxSizeInput);
    AddWidget("Cone FOV", p_generationShapeConeFOVInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Gravity multiplier", p_gravityMultiplierInput->GetGameObject());
    AddWidget("Init vel. multiplier", p_initialVelocityMultiplier->GetGameObject());
    SetLabelsWidth(100);

    EnableOnlyNeededWidgets();
}

void CIWParticleSystem::UpdateFromReference()
{
    CIWRenderer::UpdateFromReference();

    Mesh *mesh = GetParticleSystem()->GetMesh();
    Path meshPath = mesh ? mesh->GetResourceFilepath() : Path::Empty;
    p_meshInputFile->SetPath(meshPath);

    if (!p_lifeTimeInput->HasFocus())
    {
        p_lifeTimeInput->Set( GetParticleSystem()->GetLifeTime() );
    }

    if (!p_startTimeInput->HasFocus())
    {
        p_startTimeInput->Set( GetParticleSystem()->GetStartTime() );
    }

    if (!p_startSizeInput->HasFocus())
    {
        p_startSizeInput->Set( GetParticleSystem()->GetStartSize() );
    }

    p_billboardInput->SetChecked( GetParticleSystem()->GetBillboard() );

    if (!p_startColorInput->HasFocus())
    {
        p_startColorInput->SetColor( GetParticleSystem()->GetStartColor() );
    }

    if (!p_endColorInput->HasFocus())
    {
        p_endColorInput->SetColor( GetParticleSystem()->GetEndColor() );
    }

    if (!p_numParticlesInput->HasFocus())
    {
        p_numParticlesInput->SetValue( GetParticleSystem()->GetNumParticles() );
    }

    if (!p_generationShapeInput->HasFocus())
    {
        p_generationShapeInput->SetSelectionByValue(
                    SCAST<int>(GetParticleSystem()->GetGenerationShape()) );
    }

    if (!p_generationShapeBoxSizeInput->HasFocus())
    {
        p_generationShapeBoxSizeInput->Set(
                    GetParticleSystem()->GetGenerationShapeBoxSize());
    }

    if (!p_generationShapeConeFOVInput->HasFocus())
    {
        p_generationShapeConeFOVInput->SetValue(
                    Math::RadToDeg(GetParticleSystem()->
                                   GetGenerationShapeConeFOVRads()));
    }

    if (!p_gravityMultiplierInput->HasFocus())
    {
        p_gravityMultiplierInput->SetValue(
                    GetParticleSystem()->GetGravityMultiplier());
    }

    if (!p_initialVelocityMultiplier->HasFocus())
    {
        p_initialVelocityMultiplier->SetValue(
                    GetParticleSystem()->GetInitialVelocityMultiplier() );
    }

    EnableOnlyNeededWidgets();
}

void CIWParticleSystem::EnableOnlyNeededWidgets()
{
    SetWidgetEnabled(p_materialInputFile, false);
    SetWidgetEnabled(p_castsShadowsCheckBox->GetGameObject(), false);
    SetWidgetEnabled(p_receivesShadowsCheckBox->GetGameObject(), false);
    SetWidgetEnabled(p_useReflectionProbesCheckBox->GetGameObject(), false);

    Array<GameObject*> allWidgetsToEnableOrDisable;
    allWidgetsToEnableOrDisable.PushBack(p_generationShapeBoxSizeInput);
    allWidgetsToEnableOrDisable.PushBack(p_generationShapeConeFOVInput->
                                         GetGameObject());

    Array<GameObject*> keepWidgetsEnabled;
    if (GetParticleSystem())
    {
        switch (GetParticleSystem()->GetGenerationShape())
        {
            case ParticleGenerationShape::BOX:
                keepWidgetsEnabled.PushBack(p_generationShapeBoxSizeInput);
            break;

            case ParticleGenerationShape::CONE:
                keepWidgetsEnabled.PushBack(p_generationShapeConeFOVInput->
                                            GetGameObject());
            break;
        }
    }

    for (GameObject *widget : allWidgetsToEnableOrDisable)
    {
        bool enabled = (keepWidgetsEnabled.Contains(widget));
        SetWidgetEnabled(widget, enabled);
    }
}

void CIWParticleSystem::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    CIWRenderer::OnValueChangedCIW(object);

    if (object == p_meshInputFile)
    {
        Path meshPath = p_meshInputFile->GetPath();
        RH<Mesh> mesh = Resources::Load<Mesh>(meshPath);
        GetParticleSystem()->SetMesh(mesh.Get());
    }

    if (object == p_lifeTimeInput)
    {
        GetParticleSystem()->SetLifeTime( p_lifeTimeInput->GetComplexRandom() );
    }

    if (object == p_startTimeInput)
    {
        GetParticleSystem()->SetStartTime( p_startTimeInput->GetComplexRandom() );
    }

    if (object == p_billboardInput)
    {
        GetParticleSystem()->SetBillboard( p_billboardInput->IsChecked() );
    }

    if (object == p_startColorInput)
    {
        GetParticleSystem()->SetStartColor( p_startColorInput->GetColor() );
    }

    if (object == p_endColorInput)
    {
        GetParticleSystem()->SetEndColor( p_endColorInput->GetColor() );
    }

    if (object == p_numParticlesInput)
    {
        GetParticleSystem()->SetNumParticles(p_numParticlesInput->GetValue());
    }

    if (object == p_startSizeInput)
    {
        GetParticleSystem()->SetStartSize( p_startSizeInput->GetComplexRandom() );
    }

    if (object == p_generationShapeInput)
    {
        GetParticleSystem()->SetGenerationShape(
                SCAST<ParticleGenerationShape>(
                        p_generationShapeInput->GetSelectedValue()) );
    }

    GetParticleSystem()->SetGenerationShapeBoxSize(
                    p_generationShapeBoxSizeInput->GetVector3() );
    GetParticleSystem()->SetGenerationShapeConeFOVRads(
                    Math::DegToRad(p_generationShapeConeFOVInput->GetValue()) );

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
    return SCAST<ParticleSystem*>( GetRenderer() );
}
