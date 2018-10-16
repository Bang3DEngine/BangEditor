#include "BangEditor/CIWRectTransform.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/IEvents.h"
#include "Bang/RectTransform.h"
#include "BangEditor/UIInputVector.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class IEventsValueChanged;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWRectTransform::InitInnerWidgets()
{
    CIWTransform::InitInnerWidgets();

    SetName("CIWRectTransform");
    SetTitle("Rect Transform");

    p_anchorMinInput      = GameObject::Create<UIInputVector>(2);
    p_anchorMaxInput      = GameObject::Create<UIInputVector>(2);
    p_marginLeftBotInput  = GameObject::Create<UIInputVector>(2);
    p_marginRightTopInput = GameObject::Create<UIInputVector>(2);
    p_pivotPositionInput  = GameObject::Create<UIInputVector>(2);

    p_anchorMinInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_anchorMaxInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_marginLeftBotInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_marginRightTopInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_pivotPositionInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Anchor Min",       p_anchorMinInput);
    AddWidget("Anchor Max",       p_anchorMaxInput);
    AddWidget("Margin Left Bot",  p_marginLeftBotInput);
    AddWidget("Margin Right Top", p_marginRightTopInput);
    AddWidget("Pivot",            p_pivotPositionInput);

    SetLabelsWidth(110);
}

void CIWRectTransform::UpdateFromReference()
{
    CIWTransform::UpdateFromReference();

    if (!p_anchorMinInput->HasFocus())
    {
        p_anchorMinInput->Set( GetRectTransform()->GetAnchorMin() );
    }

    if (!p_anchorMaxInput->HasFocus())
    {
        p_anchorMaxInput->Set( GetRectTransform()->GetAnchorMax() );
    }

    if (!p_marginLeftBotInput->HasFocus())
    {
        p_marginLeftBotInput->Set( Vector2(GetRectTransform()->GetMarginLeftBot()) );
    }

    if (!p_marginRightTopInput->HasFocus())
    {
        p_marginRightTopInput->Set( Vector2(GetRectTransform()->GetMarginRightTop()) );
    }

    if (!p_pivotPositionInput->HasFocus())
    {
        p_pivotPositionInput->Set( GetRectTransform()->GetPivotPosition() );
    }
}

bool CIWRectTransform::CanBeRemovedFromContextMenu() const
{
    return false;
}

RectTransform *CIWRectTransform::GetRectTransform() const
{
    return SCAST<RectTransform*>( GetComponent() );
}

void CIWRectTransform::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    CIWTransform::OnValueChangedCIW(object);

    GetRectTransform()->SetAnchorMin(p_anchorMinInput->GetVector2());
    GetRectTransform()->SetAnchorMax(p_anchorMaxInput->GetVector2());
    GetRectTransform()->SetMargins(Vector2i(p_marginLeftBotInput->GetVector2()),
                                   Vector2i(p_marginRightTopInput->GetVector2()));
    GetRectTransform()->SetPivotPosition(p_pivotPositionInput->GetVector2());
}

bool CIWRectTransform::MustShowEnabledCheckbox() const
{
    return false;
}

