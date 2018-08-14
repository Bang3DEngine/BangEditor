#ifndef EDITORRESOURCES_H
#define EDITORRESOURCES_H

#include "Bang/Resources.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ModelPreviewFactory;
FORWARD class MaterialPreviewFactory;

class EditorResources : public Resources
{
public:
	EditorResources();
	virtual ~EditorResources();

    ModelPreviewFactory *GetModelPreviewFactory() const;
    MaterialPreviewFactory *GetMaterialPreviewFactory() const;

    static EditorResources *GetInstance();

    virtual void Init() override;
    void InitAfterGLIsInited();

private:
    ModelPreviewFactory *m_modelPreviewFactory = nullptr;
    MaterialPreviewFactory *m_materialPreviewFactory = nullptr;

    virtual MeshFactory* CreateMeshFactory() const override;
    virtual TextureFactory* CreateTextureFactory() const override;

    Array<Path> GetLookUpPaths() const override;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORRESOURCES_H

