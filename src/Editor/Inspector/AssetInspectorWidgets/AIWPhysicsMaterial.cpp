#include "BangEditor/AIWPhysicsMaterial.h"

#include "Bang/EventEmitter.h"
#include "Bang/Extensions.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIComboBox.h"
#include "Bang/UISlider.h"
#include "BangEditor/AIWAsset.tcc"
#include "BangEditor/EditorTextureFactory.h"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
}

using namespace Bang;
using namespace BangEditor;

AIWPhysicsMaterial::AIWPhysicsMaterial()
{
}

AIWPhysicsMaterial::~AIWPhysicsMaterial()
{
}

void AIWPhysicsMaterial::Init()
{
    AIWAsset<PhysicsMaterial>::Init();

    SetName("AIWPhysicsMaterial");
    SetTitle("Physics Material");

    p_staticFrictionInput = GameObjectFactory::CreateUISlider();
    p_staticFrictionInput->SetMinMaxValues(0.0f, 1.0f);
    p_staticFrictionInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_dynamicFrictionInput = GameObjectFactory::CreateUISlider();
    p_dynamicFrictionInput->SetMinMaxValues(0.0f, 1.0f);
    p_dynamicFrictionInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_restitutionInput = GameObjectFactory::CreateUISlider();
    p_restitutionInput->SetMinMaxValues(0.0f, 1.0f);
    p_restitutionInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    using CM = PhysicsMaterial::CombineMode;
    p_frictionCombineModeInput = GameObjectFactory::CreateUIComboBox();
    p_frictionCombineModeInput->AddItem("Average", SCAST<int>(CM::AVERAGE));
    p_frictionCombineModeInput->AddItem("Min", SCAST<int>(CM::MIN));
    p_frictionCombineModeInput->AddItem("Multiply", SCAST<int>(CM::MULTIPLY));
    p_frictionCombineModeInput->AddItem("Max", SCAST<int>(CM::MAX));
    p_frictionCombineModeInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_restitutionCombineModeInput = GameObjectFactory::CreateUIComboBox();
    p_restitutionCombineModeInput->AddItem("Average", SCAST<int>(CM::AVERAGE));
    p_restitutionCombineModeInput->AddItem("Min", SCAST<int>(CM::MIN));
    p_restitutionCombineModeInput->AddItem("Multiply",
                                           SCAST<int>(CM::MULTIPLY));
    p_restitutionCombineModeInput->AddItem("Max", SCAST<int>(CM::MAX));
    p_restitutionCombineModeInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Static Friction", p_staticFrictionInput->GetGameObject());
    AddWidget("Dynamic Friction", p_dynamicFrictionInput->GetGameObject());
    AddWidget("Restitution", p_restitutionInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 10);
    AddWidget("Friction Combine", p_frictionCombineModeInput->GetGameObject());
    AddWidget("Restitution Combine",
              p_restitutionCombineModeInput->GetGameObject());

    SetLabelsWidth(130);
}

PhysicsMaterial *AIWPhysicsMaterial::GetPhysicsMaterial() const
{
    return SCAST<PhysicsMaterial *>(GetAsset().Get());
}

void AIWPhysicsMaterial::UpdateInputsFromAsset()
{
    p_staticFrictionInput->SetValue(GetPhysicsMaterial()->GetStaticFriction());
    p_dynamicFrictionInput->SetValue(
        GetPhysicsMaterial()->GetDynamicFriction());
    p_restitutionInput->SetValue(GetPhysicsMaterial()->GetRestitution());
    p_frictionCombineModeInput->SetSelectionByValue(
        SCAST<int>(GetPhysicsMaterial()->GetFrictionCombineMode()));
    p_restitutionCombineModeInput->SetSelectionByValue(
        SCAST<int>(GetPhysicsMaterial()->GetRestitutionCombineMode()));
}

Texture2D *AIWPhysicsMaterial::GetIconTexture() const
{
    return EditorTextureFactory::GetIconForExtension(
        Extensions::GetPhysicsMaterialExtension());
}

void AIWPhysicsMaterial::OnValueChangedAIWAsset(
    EventEmitter<IEventsValueChanged> *object)
{
    GetPhysicsMaterial()->SetStaticFriction(p_staticFrictionInput->GetValue());
    GetPhysicsMaterial()->SetDynamicFriction(
        p_dynamicFrictionInput->GetValue());
    GetPhysicsMaterial()->SetRestitution(p_restitutionInput->GetValue());
    GetPhysicsMaterial()->SetFrictionCombineMode(
        SCAST<PhysicsMaterial::CombineMode>(
            p_frictionCombineModeInput->GetSelectedValue()));
    GetPhysicsMaterial()->SetRestitutionCombineMode(
        SCAST<PhysicsMaterial::CombineMode>(
            p_restitutionCombineModeInput->GetSelectedValue()));
}
