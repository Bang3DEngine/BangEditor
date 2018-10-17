#ifndef CIWWATERRENDERER_H
#define CIWWATERRENDERER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWRenderer.h"

namespace Bang
{
class IEventsValueChanged;
class WaterRenderer;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
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
}

#endif  // CIWWATERRENDERER_H
