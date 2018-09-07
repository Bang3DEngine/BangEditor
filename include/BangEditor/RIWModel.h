#ifndef RIWMODEL_H
#define RIWMODEL_H

#include "Bang/Path.h"
#include "Bang/Model.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/RIWResource.h"

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

    RIWModel();
    virtual ~RIWModel();

    Model *GetModel() const;

    // RIWResource
    void UpdateInputsFromResource() override;
    Texture2D *GetIconTexture() const override;

    // RIWResource
    void OnValueChangedRIWResource(EventEmitter<IEventsValueChanged> *object) override;

    friend class ResourceInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // RIWMODEL_H

