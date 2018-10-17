#ifndef RIWMODEL_H
#define RIWMODEL_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Model.h"
#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/RIWResource.h"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
class UIInputNumber;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class PreviewViewer;

class RIWModel : public RIWResource<Model>
{
    GAMEOBJECT_EDITOR(RIWModel);

public:
    // InspectorWidget
    void Init() override;

private:
    PreviewViewer *p_modelPreviewViewer = nullptr;

    UIInputNumber *p_numMeshes = nullptr;
    UIInputNumber *p_numVertices = nullptr;
    UIInputNumber *p_numTriangles = nullptr;
    UIInputNumber *p_numBones = nullptr;
    UIInputNumber *p_numMaterials = nullptr;
    UIInputNumber *p_numAnimations = nullptr;

    RIWModel();
    virtual ~RIWModel() override;

    Model *GetModel() const;

    // RIWResource
    void OnResourceSet() override;
    void UpdateInputsFromResource() override;
    Texture2D *GetIconTexture() const override;

    // RIWResource
    void OnValueChangedRIWResource(
        EventEmitter<IEventsValueChanged> *object) override;

    friend class ResourceInspectorWidgetFactory;
};
}

#endif  // RIWMODEL_H
