#include "BangEditor/UIInputColor.h"

#include "Bang/UIButton.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/EditorDialog.h"
#include "BangEditor/UIInputVector.h"
#include "BangEditor/EditorIconManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIInputColor::UIInputColor()
{
    SetName("UIInputColor");
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalStretch(Stretch::Full);
    hl->SetSpacing(5);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleWidth(1.0f);

    p_colorImage = GameObjectFactory::CreateUIImage(Color::Black);
    p_colorImage->SetMode(UIImageRenderer::Mode::SLICE_9);
    p_colorImage->SetImageTexture( Resources::Load<Texture2D>(
                                    EPATH("Images/RRect_9s.png") ).Get() );
    UILayoutElement *colorImgLE = p_colorImage->GetGameObject()->
                                       AddComponent<UILayoutElement>();
    colorImgLE->SetPreferredWidth(20);

    p_searchColorButton = GameObjectFactory::CreateUIButton();
    p_searchColorButton->GetText()->SetContent("");
    p_searchColorButton->SetIcon( EditorIconManager::GetLensLittleIcon().Get(),
                                  Vector2i(16) );
    p_searchColorButton->GetFocusable()->AddClickedCallback([this](IFocusable*)
    {
        // Color color = EditorDialog::GetAsset("Pick Color...", {}});
    });

    p_colorInputVector = GameObject::Create<UIInputVector>();
    p_colorInputVector->SetSize(4);
    p_colorInputVector->EventEmitter<IValueChangedListener>::RegisterListener(this);
    UILayoutElement *inputVectorLE = p_colorInputVector->AddComponent<UILayoutElement>();
    inputVectorLE->SetFlexibleWidth(1.0f);

    for (int i = 0; i < 4; ++i)
    {
        p_colorInputVector->GetInputNumbers()[i]->SetMinMaxValues(0.0f, 1.0f);
    }

    p_colorInputVector->SetParent(this);
    p_colorImage->GetGameObject()->SetParent(this);
    p_searchColorButton->GetGameObject()->SetParent(this);
}

UIInputColor::~UIInputColor()
{
}

void UIInputColor::OnValueChanged(Object *object)
{
    SetColor( Color::FromVector4(p_colorInputVector->GetVector4()) );
}

void UIInputColor::SetColor(const Color &color)
{
    if (color != GetColor())
    {
        m_color = color;

        p_colorImage->SetTint( GetColor() );
        p_colorInputVector->Set( GetColor().ToVector4() );

        EventEmitter<IValueChangedListener>::PropagateToListeners(
                     &IValueChangedListener::OnValueChanged, this);
    }

}

const Color &UIInputColor::GetColor() const
{
    return m_color;
}

bool UIInputColor::HasFocus() const
{
    return p_colorInputVector->HasFocus();
}

