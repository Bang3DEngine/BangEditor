#ifndef CIWROPE_H
#define CIWROPE_H

#include "Bang/Bang.h"

#include "BangEditor/CIWRenderer.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class CIWRope : public CIWRenderer
{
    GAMEOBJECT_EDITOR(CIWRope);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

private:
	CIWRope();
	virtual ~CIWRope();
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWROPE_H

