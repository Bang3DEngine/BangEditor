#ifndef EDITORASSETS_H
#define EDITORASSETS_H

#include "Bang/Array.h"
#include "Bang/Assets.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class MeshFactory;
class Path;
class TextureFactory;
}

using namespace Bang;
namespace BangEditor
{
class MaterialPreviewFactory;
class MeshPreviewFactory;
class ModelPreviewFactory;

class EditorAssets : public Assets
{
public:
    EditorAssets();
    virtual ~EditorAssets() override;

    MeshPreviewFactory *GetMeshPreviewFactory() const;
    ModelPreviewFactory *GetModelPreviewFactory() const;
    MaterialPreviewFactory *GetMaterialPreviewFactory() const;

    static EditorAssets *GetInstance();

    virtual void Init() override;
    void InitAfterGLIsInited();

private:
    MeshPreviewFactory *m_meshPreviewFactory = nullptr;
    ModelPreviewFactory *m_modelPreviewFactory = nullptr;
    MaterialPreviewFactory *m_materialPreviewFactory = nullptr;

    virtual MeshFactory *CreateMeshFactory() const override;
    virtual TextureFactory *CreateTextureFactory() const override;

    Array<Path> GetLookUpPaths() const override;
};
}

#endif  // EDITORASSETS_H
