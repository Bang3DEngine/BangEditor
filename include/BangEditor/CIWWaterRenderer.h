#ifndef CIWWATERRENDERER_H
#define CIWWATERRENDERER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWRenderer.h"

namespace Bang {
class IEventsValueChanged;
template <class > class EventEmitter;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class WaterRenderer;

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
	virtual ~CIWWaterRenderer();

    WaterRenderer *GetWaterRenderer() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWWATERRENDERER_H

