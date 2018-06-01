#ifndef CIWRECTTRANSFORM_H
#define CIWRECTTRANSFORM_H

#include "Bang/IEventsValueChanged.h"

#include "BangEditor/CIWTransform.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class RectTransform;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputVector;

class CIWRectTransform : public CIWTransform
{
    GAMEOBJECT_EDITOR(CIWRectTransform);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    virtual bool CanBeRemovedFromContextMenu() const override;

    RectTransform *GetRectTransform() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    // ComponentWidget
    bool MustShowEnabledCheckbox() const override;

private:
    UIInputVector *p_anchorMinInput = nullptr;
    UIInputVector *p_anchorMaxInput = nullptr;

    UIInputVector *p_marginLeftBotInput  = nullptr;
    UIInputVector *p_marginRightTopInput = nullptr;

    UIInputVector *p_pivotPositionInput = nullptr;

    CIWRectTransform() = default;
    virtual ~CIWRectTransform() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWRECTTRANSFORM_H

