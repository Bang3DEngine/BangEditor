#ifndef EDITORRESOURCES_H
#define EDITORRESOURCES_H

#include "Bang/Resources.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class MaterialPreviewFactory;

class EditorResources : public Resources
{
public:
	EditorResources();
	virtual ~EditorResources();

    MaterialPreviewFactory *GetMaterialPreviewFactory() const;

    static EditorResources *GetInstance();

private:
    MaterialPreviewFactory *m_materialPreviewFactory = nullptr;

    virtual MeshFactory* CreateMeshFactory() const override;

    Array<Path> GetLookUpPaths() const override;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORRESOURCES_H

