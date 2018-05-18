#include "BangEditor/CIWPostProcessEffect.h"

#include "Bang/Shader.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/PostProcessEffect.h"

#include "BangEditor/UIInputFile.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWPostProcessEffect::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWPostProcessEffect");
    SetTitle("PostProcessEffect");

    p_priorityInput = GameObjectFactory::CreateUIInputNumber();
    p_priorityInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_priorityInput->SetDecimalPlaces(0);

    p_fragmentShaderInput = GameObject::Create<UIInputFile>();
    p_fragmentShaderInput->SetExtensions(
                            Extensions::GetFragmentShaderExtensions() );
    p_fragmentShaderInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    p_typeComboBox = GameObjectFactory::CreateUIComboBox();
    p_typeComboBox->AddItem("AfterScene",
                            SCAST<int>(PostProcessEffect::Type::AfterScene));
    p_typeComboBox->AddItem("AfterCanvas",
                            SCAST<int>(PostProcessEffect::Type::AfterCanvas));
    p_typeComboBox->EventEmitter<IValueChangedListener>::RegisterListener(this);

    AddWidget("Priority", p_priorityInput->GetGameObject());
    AddWidget("Shader", p_fragmentShaderInput);
    AddWidget("Type", p_typeComboBox->GetGameObject());
}

void CIWPostProcessEffect::UpdateFromReference()
{
    PostProcessEffect *ppe = GetPostProcessEffect();

    p_priorityInput->SetValue( SCAST<float>( ppe->GetPriority() ) );
    p_fragmentShaderInput->SetPath( ppe->GetPostProcessShaderFilepath() );
    p_typeComboBox->SetSelectionByValue( SCAST<int>(ppe->GetType()) );
}

PostProcessEffect *CIWPostProcessEffect::GetPostProcessEffect() const
{
    return SCAST<PostProcessEffect*>( GetComponent() );
}

void CIWPostProcessEffect::OnValueChanged(Object *object)
{
    ComponentInspectorWidget::OnValueChanged(object);

    PostProcessEffect *ppe = GetPostProcessEffect();

    ppe->SetType(
        SCAST<PostProcessEffect::Type>(p_typeComboBox->GetSelectedValue()) );
    ppe->SetPriority( SCAST<int>( p_priorityInput->GetValue() ) );

    if (p_fragmentShaderInput->GetPath().IsFile())
    {
        const Path fragPath = p_fragmentShaderInput->GetPath();
        RH<Shader> shader = Resources::Load<Shader>(fragPath);
        ppe->SetPostProcessShader( shader.Get() );
    }
    else
    {
        ppe->SetPostProcessShader( nullptr );
    }
}
