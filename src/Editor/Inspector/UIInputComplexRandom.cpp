#include "BangEditor/UIInputComplexRandom.h"

#include "Bang/ComplexRandom.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UILayoutElement.h"

using namespace Bang;
using namespace BangEditor;

UIInputComplexRandom::UIInputComplexRandom()
{
    SetName("UIInputComplexRandom");

    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetSpacing(5);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize(Vector2::One());

    p_minRangeInputNumber = GameObjectFactory::CreateUIInputNumber();
    p_maxRangeInputNumber = GameObjectFactory::CreateUIInputNumber();
    p_minRangeInputNumber->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    p_maxRangeInputNumber->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_minRangeInputNumber->GetGameObject()->SetParent(this);
    p_maxRangeInputNumber->GetGameObject()->SetParent(this);

    Set(ComplexRandom(0, 1));
}

UIInputComplexRandom::~UIInputComplexRandom()
{
}

void UIInputComplexRandom::Set(const ComplexRandom &complexRandom)
{
    if (complexRandom != GetComplexRandom())
    {
        m_complexRandom = complexRandom;

        p_minRangeInputNumber->SetValue(GetComplexRandom().GetMinRangeValue());
        p_maxRangeInputNumber->SetValue(GetComplexRandom().GetMaxRangeValue());

        p_minRangeInputNumber->SetMaxValue(
            GetComplexRandom().GetMaxRangeValue());
        p_maxRangeInputNumber->SetMinValue(
            GetComplexRandom().GetMinRangeValue());
    }
}

void UIInputComplexRandom::SetRangeMinValue(float minValue)
{
    p_minRangeInputNumber->SetMinValue(minValue);
    p_maxRangeInputNumber->SetMinValue(minValue);
}

void UIInputComplexRandom::SetRangeMaxValue(float maxValue)
{
    p_minRangeInputNumber->SetMaxValue(maxValue);
    p_maxRangeInputNumber->SetMaxValue(maxValue);
}

void UIInputComplexRandom::SetRangeMinMaxValues(float minValue, float maxValue)
{
    SetRangeMinValue(minValue);
    SetRangeMaxValue(maxValue);
}

bool UIInputComplexRandom::HasFocus() const
{
    return p_minRangeInputNumber->HasFocus() ||
           p_maxRangeInputNumber->HasFocus();
}

const ComplexRandom &UIInputComplexRandom::GetComplexRandom() const
{
    return m_complexRandom;
}

void UIInputComplexRandom::OnValueChanged(EventEmitter<IEventsValueChanged> *)
{
    ComplexRandom cr(p_minRangeInputNumber->GetValue(),
                     p_maxRangeInputNumber->GetValue());
    Set(cr);

    EventEmitter<IEventsValueChanged>::PropagateToListeners(
        &IEventsValueChanged::OnValueChanged, this);
}
