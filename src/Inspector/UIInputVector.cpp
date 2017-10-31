#include "BangEditor/UIInputVector.h"

#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/UILabel.h"
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
    labelLE->SetFlexibleWidth(0);

    m_inputNumbers.PushBack( GameObjectFactory::CreateUIInputNumber() );
    m_inputNumbers.PushBack( GameObjectFactory::CreateUIInputNumber() );
    m_inputNumbers.PushBack( GameObjectFactory::CreateUIInputNumber() );
    m_inputNumbers.PushBack( GameObjectFactory::CreateUIInputNumber() );

    AddChild(p_label->GetGameObject());
    AddChild(m_inputNumbers[0]->GetGameObject());
    AddChild(m_inputNumbers[1]->GetGameObject());
    AddChild(m_inputNumbers[2]->GetGameObject());
    AddChild(m_inputNumbers[3]->GetGameObject());
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

    m_values.Resize(size);
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
    m_values[i] = v;
    m_inputNumbers[i]->SetNumber(v);
}

Vector2 UIInputVector::GetVector2() const
{
    return Vector2(m_values[0], m_values[1]);
}
Vector3 UIInputVector::GetVector3() const
{
    return Vector3(m_values[0], m_values[1], m_values[2]);
}
Vector4 UIInputVector::GetVector4() const
{
    return Vector4(m_values[0], m_values[1], m_values[2], m_values[3]);
}

