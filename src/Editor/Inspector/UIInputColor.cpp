#include "BangEditor/UIInputColor.h"

#include "Bang/UIButton.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/IconManager.h"
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

    GameObject *colorImgCont = GameObjectFactory::CreateUIGameObject();
    UIFocusable *colorImgFocusable = colorImgCont->AddComponent<UIFocusable>();
    colorImgFocusable->SetCursorType(Cursor::Type::Hand);
    UILayoutElement *colorImgLE = colorImgCont->AddComponent<UILayoutElement>();
    colorImgLE->SetFlexibleWidth(1.0f);

    p_bgCheckerboardImage = colorImgCont->AddComponent<UIImageRenderer>();
    p_bgCheckerboardImage->SetImageTexture( IconManager::GetCheckerboard().Get() );
    p_bgCheckerboardImage->GetMaterial()->SetAlbedoUvMultiply( Vector2(1, 1) );

    p_colorImage = GameObjectFactory::CreateUIImage();

    m_colorPickerReporter = new ColorPickerReporter();
    m_colorPickerReporter->EventEmitter<IValueChangedListener>::RegisterListener(this);

    RH<Texture2D> lensIcon = EditorIconManager::GetLensLittleIcon().Get();
    p_searchColorButton = GameObjectFactory::CreateUIButton("", lensIcon.Get());
    p_searchColorButton->SetIcon(lensIcon.Get(), Vector2i(16));
    p_searchColorButton->GetFocusable()->AddClickedCallback([this](IFocusable*)
    {
        EditorDialog::GetColor("Pick Color...", GetColor(), m_colorPickerReporter);
    });

    colorImgFocusable->AddClickedCallback([this](IFocusable*)
    {
        p_searchColorButton->Click(false);
    });

    colorImgCont->SetParent(this);
    p_searchColorButton->GetGameObject()->SetParent(this);
    p_colorImage->GetGameObject()->SetParent(colorImgCont);
}

UIInputColor::~UIInputColor()
{
    Object::DestroyObject(m_colorPickerReporter);
}

void UIInputColor::Update()
{
    GameObject::Update();
    SetColor( m_colorPickerReporter->GetPickedColor() );
}

void UIInputColor::OnValueChanged(Object *object)
{
    // Dont do anything here, since this is being called from the
    // color picker window loop, and nasty things can happen
}

void UIInputColor::SetColor(const Color &color)
{
    if (color != GetColor())
    {
        m_color = color;
        p_colorImage->SetTint( GetColor() );
        m_colorPickerReporter->SetPickedColor( GetColor() );

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
    return !m_colorPickerReporter->HasFinished();
}

