#include "BangEditor/UIInputVector.h"

#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/UILabel.h"
#include "Bang/UICanvas.h"
#include "Bang/UIInputText.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIInputVector::UIInputVector()
{
    SetName("UIInputVector");

    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetSpacing(5);

    p_label = GameObjectFactory::CreateUILabel();
    p_label->GetText()->SetContent("Label");
    p_label->GetText()->SetTextSize(11);
    p_label->GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    UILayoutElement *labelLE =
                  p_label->GetGameObject()->GetComponent<UILayoutElement>();
    p_label->SetSelectable(false);
    labelLE->SetFlexibleWidth(0);

    m_inputNumbers.PushBack( GameObjectFactory::CreateUIInputNumber() );
    m_inputNumbers.PushBack( GameObjectFactory::CreateUIInputNumber() );
    m_inputNumbers.PushBack( GameObjectFactory::CreateUIInputNumber() );
    m_inputNumbers.PushBack( GameObjectFactory::CreateUIInputNumber() );

    for (int i = 0; i < 4; ++i)
    {
        m_inputNumbers[i]->EventEmitter<IValueChangedListener>::RegisterListener(this);
    }

    p_label->GetGameObject()->SetParent(this);
    m_inputNumbers[0]->GetGameObject()->SetParent(this);
    m_inputNumbers[1]->GetGameObject()->SetParent(this);
    m_inputNumbers[2]->GetGameObject()->SetParent(this);
    m_inputNumbers[3]->GetGameObject()->SetParent(this);
}

UIInputVector::UIInputVector(const String &labelText, int size) : UIInputVector()
{
    SetName("UIInputVector_" + labelText);
    SetLabelText(labelText);
    SetSize(size);
}

UIInputVector::~UIInputVector()
{
}

void UIInputVector::SetLabelText(const String &text)
{
    p_label->GetText()->SetContent(text);
}

void UIInputVector::SetSize(int size)
{
    ASSERT(size >= 1 && size <= 4);
    for (int i = 0; i < 4; ++i)
    {
        m_inputNumbers[i]->GetGameObject()->SetEnabled( i < size );
    }
}

void UIInputVector::Set(const Vector2 &v)
{
    for (int i = 0; i < 2; ++i) { Set(i, v[i]); }
}
void UIInputVector::Set(const Vector3 &v)
{
    for (int i = 0; i < 3; ++i) { Set(i, v[i]); }
}
void UIInputVector::Set(const Vector4 &v)
{
    for (int i = 0; i < 4; ++i) { Set(i, v[i]); }
}
void UIInputVector::Set(int i, float v)
{
    m_inputNumbers[i]->SetNumber(v);
}

float UIInputVector::Get(int i) const
{
    return m_inputNumbers[i]->GetNumber();
}

Vector2 UIInputVector::GetVector2() const
{
    return Vector2(Get(0), Get(1));
}
Vector3 UIInputVector::GetVector3() const
{
    return Vector3(Get(0), Get(1), Get(2));
}
Vector4 UIInputVector::GetVector4() const
{
    return Vector4(Get(0), Get(1), Get(2), Get(3));
}

bool UIInputVector::HasFocus() const
{
    return m_inputNumbers[0]->HasFocus() || m_inputNumbers[1]->HasFocus() ||
           m_inputNumbers[2]->HasFocus() || m_inputNumbers[3]->HasFocus();
}

void UIInputVector::SetPreferredWidth(int width)
{
    p_label->GetGameObject()->GetComponent<UILayoutElement>()->
                                        SetPreferredWidth(width);
}

void UIInputVector::OnValueChanged(Object *object)
{
    EventEmitter<IValueChangedListener>::
        PropagateToListeners(&IValueChangedListener::OnValueChanged, object);
}

