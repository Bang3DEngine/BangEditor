#include "BangEditor/CIWPostProcessEffect.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/Path.h"
#include "Bang/PostProcessEffect.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
#include "Bang/Shader.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "BangEditor/UIInputFileWithPreview.h"

namespace Bang {
class IEventsValueChanged;
}  // namespace Bang

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWPostProcessEffect::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWPostProcessEffect");
    SetTitle("PostProcessEffect");

    p_priorityInput = GameObjectFactory::CreateUIInputNumber();
    p_priorityInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_priorityInput->SetDecimalPlaces(0);

    p_fragmentShaderInput = GameObject::Create<UIInputFileWithPreview>();
    p_fragmentShaderInput->SetExtensions(
                            Extensions::GetFragmentShaderExtensions() );
    p_fragmentShaderInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_fragmentShaderInput->SetZoomable(false);

    p_typeComboBox = GameObjectFactory::CreateUIComboBox();
    p_typeComboBox->AddItem("AfterScene",
                            SCAST<int>(PostProcessEffect::Type::AFTER_SCENE));
    p_typeComboBox->AddItem("AfterCanvas",
                            SCAST<int>(PostProcessEffect::Type::AFTER_CANVAS));
    p_typeComboBox->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Priority", p_priorityInput->GetGameObject());
    AddWidget("Shader", p_fragmentShaderInput);
    AddWidget("Type", p_typeComboBox->GetGameObject());
}

void CIWPostProcessEffect::UpdateFromReference()
{
    PostProcessEffect *ppe = GetPostProcessEffect();

    p_priorityInput->SetValue( ppe->GetPriority() );
    p_fragmentShaderInput->SetPath( ppe->GetPostProcessShaderFilepath() );
    p_typeComboBox->SetSelectionByValue( SCAST<int>(ppe->GetType()) );
}

PostProcessEffect *CIWPostProcessEffect::GetPostProcessEffect() const
{
    return SCAST<PostProcessEffect*>( GetComponent() );
}

void CIWPostProcessEffect::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    PostProcessEffect *ppe = GetPostProcessEffect();

    ppe->SetType(
        SCAST<PostProcessEffect::Type>(p_typeComboBox->GetSelectedValue()) );
    ppe->SetPriority( p_priorityInput->GetValue() );

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
