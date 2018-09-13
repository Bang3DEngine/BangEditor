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

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize( Vector2::One );

    m_inputNumbers.PushBack( GameObjectFactory::CreateUIInputNumber() );
    m_inputNumbers.PushBack( GameObjectFactory::CreateUIInputNumber() );
    m_inputNumbers.PushBack( GameObjectFactory::CreateUIInputNumber() );
    m_inputNumbers.PushBack( GameObjectFactory::CreateUIInputNumber() );

    for (int i = 0; i < 4; ++i)
    {
        m_inputNumbers[i]->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    }

    m_inputNumbers[0]->GetGameObject()->SetParent(this);
    m_inputNumbers[1]->GetGameObject()->SetParent(this);
    m_inputNumbers[2]->GetGameObject()->SetParent(this);
    m_inputNumbers[3]->GetGameObject()->SetParent(this);
}

UIInputVector::UIInputVector(int size) : UIInputVector()
{
    SetSize(size);
}

UIInputVector::~UIInputVector()
{
}

void UIInputVector::SetSize(int size)
{
    ASSERT(size >= 1 && size <= 4);
    m_size = size;
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
    m_inputNumbers[i]->SetValue(v);
}

float UIInputVector::Get(int i) const
{
    return m_inputNumbers[i]->GetValue();
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

int UIInputVector::GetSize() const
{
    return m_size;
}

const Array<UIInputNumber *> UIInputVector::GetInputNumbers() const
{
    return m_inputNumbers;
}

bool UIInputVector::HasFocus() const
{
    return m_inputNumbers[0]->HasFocus() || m_inputNumbers[1]->HasFocus() ||
           m_inputNumbers[2]->HasFocus() || m_inputNumbers[3]->HasFocus();
}

void UIInputVector::OnValueChanged(EventEmitter<IEventsValueChanged> *object)
{
    EventEmitter<IEventsValueChanged>::
        PropagateToListeners(&IEventsValueChanged::OnValueChanged,
                             object);
}

