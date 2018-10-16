#ifndef EDITORRESOURCES_H
#define EDITORRESOURCES_H

#include "Bang/Array.h"
#include "Bang/BangDefines.h"
#include "Bang/Resources.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class MeshFactory;
FORWARD class Path;
FORWARD class TextureFactory;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class MaterialPreviewFactory;
FORWARD class MeshPreviewFactory;
FORWARD class ModelPreviewFactory;

class EditorResources : public Resources
{
public:
	EditorResources();
	virtual ~EditorResources() override;

    MeshPreviewFactory *GetMeshPreviewFactory() const;
    ModelPreviewFactory *GetModelPreviewFactory() const;
    MaterialPreviewFactory *GetMaterialPreviewFactory() const;

    static EditorResources *GetInstance();

    virtual void Init() override;
    void InitAfterGLIsInited();

private:
    MeshPreviewFactory *m_meshPreviewFactory = nullptr;
    ModelPreviewFactory *m_modelPreviewFactory = nullptr;
    MaterialPreviewFactory *m_materialPreviewFactory = nullptr;

    virtual MeshFactory* CreateMeshFactory() const override;
    virtual TextureFactory* CreateTextureFactory() const override;

    Array<Path> GetLookUpPaths() const override;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORRESOURCES_H

