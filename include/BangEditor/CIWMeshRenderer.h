#ifndef CIWMESHRENDERER_H
#define CIWMESHRENDERER_H

#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWRenderer.h"

namespace Bang
{
class IEventsValueChanged;
class MeshRenderer;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFileWithPreview;

class CIWMeshRenderer : public CIWRenderer
{
    GAMEOBJECT_EDITOR(CIWMeshRenderer);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    UIInputFileWithPreview *p_meshInputFile = nullptr;

    CIWMeshRenderer() = default;
    virtual ~CIWMeshRenderer() override = default;

    MeshRenderer *GetMeshRenderer() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;
};
}

#endif  // CIWMESHRENDERER_H
