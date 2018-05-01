#ifndef MATERIALPREVIEWFACTORY_H
#define MATERIALPREVIEWFACTORY_H

#include "Bang/Bang.h"
#include "Bang/Material.h"
#include "Bang/Texture2D.h"
#include "Bang/IMaterialChangedListener.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Camera;
FORWARD class MeshRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MaterialPreviewFactory : public IMaterialChangedListener
{
public:
    MaterialPreviewFactory() = default;
    virtual ~MaterialPreviewFactory();

    static RH<Texture2D> GetPreviewTextureFor(Material *material);
    RH<Texture2D> GetPreviewTextureFor_(Material *material);

    void CreatePreviewScene();

    static MaterialPreviewFactory *GetActive();

private:
    Map<GUID, RH<Texture2D>> m_previewsMap;
    Array<RH<Material>> m_materials;

    Scene *m_previewScene = nullptr;
    Camera *p_previewCamera = nullptr;
    MeshRenderer *p_previewMeshRenderer = nullptr;

    void FillTextureWithPreview(Texture2D *texture, Material *material);
    virtual void OnMaterialChanged(Material *changedMaterial) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // MATERIALPREVIEWFACTORY_H

