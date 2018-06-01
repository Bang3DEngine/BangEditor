#include "BangEditor/CIWUITextRenderer.h"

#include "Bang/Font.h"
#include "Bang/UICanvas.h"
#include "Bang/Resources.h"
#include "Bang/GameObject.h"
#include "Bang/Extensions.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputText.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UITextRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputFile.h"
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

    p_horizontalAlignmentInput = GameObjectFactory::CreateUIComboBox();
    p_horizontalAlignmentInput->AddItem("Left",   int(HorizontalAlignment::LEFT));
    p_horizontalAlignmentInput->AddItem("Center", int(HorizontalAlignment::CENTER));
    p_horizontalAlignmentInput->AddItem("Right",  int(HorizontalAlignment::RIGHT));

    p_verticalAlignmentInput = GameObjectFactory::CreateUIComboBox();
    p_verticalAlignmentInput->AddItem("Top",    int(VerticalAlignment::TOP));
    p_verticalAlignmentInput->AddItem("Center", int(VerticalAlignment::CENTER));
    p_verticalAlignmentInput->AddItem("Bot",    int(VerticalAlignment::BOT));

    p_fontFileInput = GameObject::Create<UIInputFile>();
    p_fontFileInput->SetExtensions( Extensions::GetTTFExtensions() );

    p_colorInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_contentInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_sizeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_horizontalAlignmentInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_verticalAlignmentInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_fontFileInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Content", p_contentInput->GetGameObject());
    AddWidget("Size",    p_sizeInput->GetGameObject());
    AddWidget("Color",   p_colorInput);
    AddWidget("HAlign",  p_horizontalAlignmentInput->GetGameObject());
    AddWidget("VAlign",  p_verticalAlignmentInput->GetGameObject());
    AddWidget("Font",    p_fontFileInput);

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

    p_horizontalAlignmentInput->SetSelectionByValue(
                int(GetUITextRenderer()->GetHorizontalAlignment()) );
    p_verticalAlignmentInput->SetSelectionByValue(
                int(GetUITextRenderer()->GetVerticalAlignment()) );

    Font *font = GetUITextRenderer()->GetFont();
    p_fontFileInput->SetPath( font ? font->GetResourceFilepath() : Path::Empty);
}

UITextRenderer *CIWUITextRenderer::GetUITextRenderer() const
{
    return SCAST<UITextRenderer*>( GetComponent() );
}

void CIWUITextRenderer::OnValueChanged(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChanged(object);

    GetUITextRenderer()->SetContent( p_contentInput->GetText()->GetContent() );
    GetUITextRenderer()->SetTextSize( p_sizeInput->GetValue() );
    GetUITextRenderer()->SetTextColor( p_colorInput->GetColor() );
    GetUITextRenderer()->SetFont( Resources::Load<Font>(p_fontFileInput->GetPath()).Get() );
    GetUITextRenderer()->SetHorizontalAlign(
        SCAST<HorizontalAlignment>(p_horizontalAlignmentInput->GetSelectedValue()) );
    GetUITextRenderer()->SetVerticalAlign(
        SCAST<VerticalAlignment>(p_verticalAlignmentInput->GetSelectedValue()) );
}
