#include "BangEditor/EditorAssets.h"

#include "Bang/Array.tcc"
#include "Bang/Path.h"
#include "BangEditor/EditorMeshFactory.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/MaterialPreviewFactory.h"
#include "BangEditor/MeshPreviewFactory.h"
#include "BangEditor/ModelPreviewFactory.h"
#include "BangEditor/AssetPreviewFactory.tcc"

namespace Bang
{
class MeshFactory;
class TextureFactory;
}

using namespace Bang;
using namespace BangEditor;

EditorAssets::EditorAssets()
{
    m_materialPreviewFactory = new MaterialPreviewFactory();
    m_modelPreviewFactory = new ModelPreviewFactory();
    m_meshPreviewFactory = new MeshPreviewFactory();
}

EditorAssets::~EditorAssets()
{
    delete m_meshPreviewFactory;
    delete m_modelPreviewFactory;
    delete m_materialPreviewFactory;
}

MeshPreviewFactory *EditorAssets::GetMeshPreviewFactory() const
{
    return m_meshPreviewFactory;
}

ModelPreviewFactory *EditorAssets::GetModelPreviewFactory() const
{
    return m_modelPreviewFactory;
}

MaterialPreviewFactory *EditorAssets::GetMaterialPreviewFactory() const
{
    return m_materialPreviewFactory;
}

EditorAssets *EditorAssets::GetInstance()
{
    return DCAST<EditorAssets *>(Assets::GetInstance());
}

void EditorAssets::Init()
{
    Assets::Init();
}

void EditorAssets::InitAfterGLIsInited()
{
    m_meshPreviewFactory->Init();
    m_modelPreviewFactory->Init();
    m_materialPreviewFactory->Init();
}

MeshFactory *EditorAssets::CreateMeshFactory() const
{
    return new EditorMeshFactory();
}

TextureFactory *EditorAssets::CreateTextureFactory() const
{
    return new EditorTextureFactory();
}

Array<Path> EditorAssets::GetLookUpPaths() const
{
    Array<Path> lookUpPaths = Assets::GetLookUpPaths();
    lookUpPaths.PushBack(EditorPaths::GetEditorAssetsDir());
    lookUpPaths.PushBack(EditorPaths::GetEditorAssetsDir().Append("Shaders"));
    lookUpPaths.PushBack(
        EditorPaths::GetEditorAssetsDir().Append("Shaders").Append("Include"));
    return lookUpPaths;
}
