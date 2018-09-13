#ifndef RIWMESH_H
#define RIWMESH_H

#include "Bang/Path.h"
#include "Bang/Mesh.h"
#include "Bang/UIInputNumber.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/RIWResource.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class PreviewViewer;

class RIWMesh : public RIWResource<Mesh>
{
    GAMEOBJECT_EDITOR(RIWMesh);

public:
    // InspectorWidget
    void Init() override;

private:
    PreviewViewer *p_meshPreviewViewer = nullptr;
    UIInputNumber *p_numVertices  = nullptr;
    UIInputNumber *p_numTriangles = nullptr;
    UIInputNumber *p_numBones = nullptr;

    RIWMesh();
    virtual ~RIWMesh();

    Mesh *GetMesh() const;

    // RIWResource
    void OnResourceSet() override;
    void UpdateInputsFromResource() override;
    Texture2D* GetIconTexture() const override;

    // RIWResource
    void OnValueChangedRIWResource(EventEmitter<IEventsValueChanged> *object) override;

    friend class ResourceInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // RIWMESH_H

