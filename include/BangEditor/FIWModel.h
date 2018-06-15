#ifndef FIWMODEL_H
#define FIWMODEL_H

#include "Bang/Path.h"
#include "Bang/Model.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/FIWResource.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class FIWModel : public FIWResource<Model>
{
    GAMEOBJECT_EDITOR(FIWModel);

public:
    // InspectorWidget
    void Init() override;

private:
    RH<Model> m_modelRH;
    UIImageRenderer *p_modelPreviewImg = nullptr;

    FIWModel();
    virtual ~FIWModel();

    Model *GetModel() const;

    // FIWResource
    void UpdateInputsFromResource() override;

    // FIWResource
    void OnValueChangedFIWResource(EventEmitter<IEventsValueChanged> *object) override;

    friend class FileInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // FIWMODEL_H

