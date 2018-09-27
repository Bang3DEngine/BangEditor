#include "BangEditor/CIWParticleSystem.h"

#include "Bang/Mesh.h"
#include "Bang/UILabel.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIInputText.h"
#include "Bang/UITextRenderer.h"
#include "Bang/ParticleSystem.h"

#include "Bang/GameObject.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputVector.h"
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

    p_lifetimeInput = GameObjectFactory::CreateUIInputNumber();
    p_lifetimeInput->SetMinValue(0);
    p_lifetimeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_numParticlesInput = GameObjectFactory::CreateUIInputNumber();
    p_numParticlesInput->SetDecimalPlaces(0);
    p_numParticlesInput->SetMinValue(0);
    p_numParticlesInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_generationShapeInput = GameObjectFactory::CreateUIComboBox();
    p_generationShapeInput->AddItem("Box", SCAST<int>(ParticleGenerationShape::BOX));
    p_generationShapeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_generationShapeBoxSizeInput = GameObject::Create<UIInputVector>();
    p_generationShapeBoxSizeInput->SetSize(3);
    p_generationShapeBoxSizeInput->SetMinValue(Vector4::Zero);
    p_generationShapeBoxSizeInput->EventEmitter<IEventsValueChanged>::
                                   RegisterListener(this);

    p_gravityMultiplierInput = GameObjectFactory::CreateUIInputNumber();
    p_gravityMultiplierInput->EventEmitter<IEventsValueChanged>::
                              RegisterListener(this);

    p_initialVelocityMultiplier = GameObjectFactory::CreateUIInputNumber();
    p_initialVelocityMultiplier->EventEmitter<IEventsValueChanged>::
                                 RegisterListener(this);

    SetWidgetEnabled(p_materialInputFile, false);
    SetWidgetEnabled(p_castsShadowsCheckBox->GetGameObject(), false);
    SetWidgetEnabled(p_receivesShadowsCheckBox->GetGameObject(), false);
    SetWidgetEnabled(p_useReflectionProbesCheckBox->GetGameObject(), false);

    AddWidget("Mesh", p_meshInputFile);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Lifetime", p_lifetimeInput->GetGameObject());
    AddWidget("Num. Particles", p_numParticlesInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Generation shape", p_generationShapeInput->GetGameObject());
    AddWidget("Box size", p_generationShapeBoxSizeInput);
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Gravity multiplier", p_gravityMultiplierInput->GetGameObject());
    AddWidget("Init vel. multiplier", p_initialVelocityMultiplier->GetGameObject());
    SetLabelsWidth(100);
}

void CIWParticleSystem::UpdateFromReference()
{
    CIWRenderer::UpdateFromReference();

    Mesh *mesh = GetParticleSystem()->GetMesh();
    Path meshPath = mesh ? mesh->GetResourceFilepath() : Path::Empty;
    p_meshInputFile->SetPath(meshPath);

    if (!p_lifetimeInput->HasFocus())
    {
        p_lifetimeInput->SetValue( GetParticleSystem()->GetLifeTime() );
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

    if (object == p_lifetimeInput)
    {
        GetParticleSystem()->SetLifeTime( p_lifetimeInput->GetValue() );
    }

    if (object == p_numParticlesInput)
    {
        GetParticleSystem()->SetNumParticles(p_numParticlesInput->GetValue());
    }

    if (object == p_generationShapeInput)
    {
        GetParticleSystem()->SetGenerationShape(
                SCAST<ParticleGenerationShape>(
                        p_generationShapeInput->GetSelectedValue()) );
    }

    GetParticleSystem()->SetGenerationShapeBoxSize(
                    p_generationShapeBoxSizeInput->GetVector3() );

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
}

ParticleSystem *CIWParticleSystem::GetParticleSystem() const
{
    return SCAST<ParticleSystem*>( GetRenderer() );
}
