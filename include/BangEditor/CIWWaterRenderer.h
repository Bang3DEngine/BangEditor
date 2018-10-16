#ifndef CIWWATERRENDERER_H
#define CIWWATERRENDERER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWRenderer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class IEventsValueChanged;
FORWARD   class WaterRenderer;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class CIWWaterRenderer : public CIWRenderer
{
    GAMEOBJECT_EDITOR(CIWWaterRenderer);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

private:
	CIWWaterRenderer();
	virtual ~CIWWaterRenderer() override;

    WaterRenderer *GetWaterRenderer() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWWATERRENDERER_H

