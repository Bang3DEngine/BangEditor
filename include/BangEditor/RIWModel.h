#ifndef RIWMODEL_H
#define RIWMODEL_H

#include "Bang/Path.h"
#include "Bang/Model.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/RIWResource.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIInputNumber;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class PreviewViewer;

class RIWModel : public RIWResource<Model>
{
    GAMEOBJECT_EDITOR(RIWModel);

public:
    // InspectorWidget
    void Init() override;

private:
    PreviewViewer *p_modelPreviewViewer = nullptr;

    UIInputNumber *p_numMeshes = nullptr;
    UIInputNumber *p_numVertices  = nullptr;
    UIInputNumber *p_numTriangles = nullptr;
    UIInputNumber *p_numBones  = nullptr;
    UIInputNumber *p_numMaterials = nullptr;
    UIInputNumber *p_numAnimations = nullptr;

    RIWModel();
    virtual ~RIWModel();

    Model *GetModel() const;

    // RIWResource
    void OnResourceSet() override;
    void UpdateInputsFromResource() override;
    Texture2D *GetIconTexture() const override;

    // RIWResource
    void OnValueChangedRIWResource(EventEmitter<IEventsValueChanged> *object) override;

    friend class ResourceInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // RIWMODEL_H

