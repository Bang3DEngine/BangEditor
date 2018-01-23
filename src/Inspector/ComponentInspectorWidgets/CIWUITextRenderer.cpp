#include "BangEditor/CIWUITextRenderer.h"

#include "Bang/UICanvas.h"
#include "Bang/GameObject.h"
#include "Bang/UIInputText.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UITextRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputColor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWUITextRenderer::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWUITextRenderer");
    SetTitle("UI Text Renderer");

    p_contentInput = GameObjectFactory::CreateUIInputText();

    p_sizeInput = GameObjectFactory::CreateUIInputNumber();

    p_colorInput = GameObject::Create<UIInputColor>();

    p_colorInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_contentInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_sizeInput->EventEmitter<IValueChangedListener>::RegisterListener(this);

    AddWidget("Content",  p_contentInput->GetGameObject());
    AddWidget("Size",     p_sizeInput->GetGameObject());
    AddWidget("Color",    p_colorInput);

    SetLabelsWidth(80);
}

void CIWUITextRenderer::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    if (!UICanvas::GetActive(p_contentInput)->HasFocus(p_contentInput))
    {
        p_contentInput->GetText()->SetContent( GetUITextRenderer()->GetContent() );
    }

    if (!p_sizeInput->HasFocus())
    {
        p_sizeInput->SetValue( GetUITextRenderer()->GetTextSize() );
    }

    if (!p_colorInput->HasFocus())
    {
        p_colorInput->SetColor( GetUITextRenderer()->GetTextColor() );
    }

}

UITextRenderer *CIWUITextRenderer::GetUITextRenderer() const
{
    return SCAST<UITextRenderer*>( GetComponent() );
}

void CIWUITextRenderer::OnValueChanged(Object *object)
{
    ComponentInspectorWidget::OnValueChanged(object);

    GetUITextRenderer()->SetContent( p_contentInput->GetText()->GetContent() );
    GetUITextRenderer()->SetTextSize( p_sizeInput->GetValue() );
    GetUITextRenderer()->SetTextColor( p_colorInput->GetColor() );
}
