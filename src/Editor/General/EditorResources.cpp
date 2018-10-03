#include "BangEditor/EditorResources.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorMeshFactory.h"
#include "BangEditor/MeshPreviewFactory.h"
#include "BangEditor/ModelPreviewFactory.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/MaterialPreviewFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorResources::EditorResources()
{
    m_materialPreviewFactory = new MaterialPreviewFactory();
    m_modelPreviewFactory = new ModelPreviewFactory();
    m_meshPreviewFactory = new MeshPreviewFactory();
}

EditorResources::~EditorResources()
{
    delete m_meshPreviewFactory;
    delete m_modelPreviewFactory;
    delete m_materialPreviewFactory;
}

MeshPreviewFactory *EditorResources::GetMeshPreviewFactory() const
{
    return m_meshPreviewFactory;
}

ModelPreviewFactory *EditorResources::GetModelPreviewFactory() const
{
    return m_modelPreviewFactory;
}

MaterialPreviewFactory *EditorResources::GetMaterialPreviewFactory() const
{
    return m_materialPreviewFactory;
}

EditorResources *EditorResources::GetInstance()
{
    return DCAST<EditorResources*>( Resources::GetInstance() );
}

void EditorResources::Init()
{
    Resources::Init();
}

void EditorResources::InitAfterGLIsInited()
{
    m_meshPreviewFactory->Init();
    m_modelPreviewFactory->Init();
    m_materialPreviewFactory->Init();
}

MeshFactory *EditorResources::CreateMeshFactory() const
{
    return new EditorMeshFactory();
}

TextureFactory *EditorResources::CreateTextureFactory() const
{
    return new EditorTextureFactory();
}

Array<Path> EditorResources::GetLookUpPaths() const
{
    Array<Path> lookUpPaths = Resources::GetLookUpPaths();
    lookUpPaths.PushBack(EditorPaths::GetEditorAssetsDir());
    lookUpPaths.PushBack(EditorPaths::GetEditorAssetsDir().Append("Shaders"));
    lookUpPaths.PushBack(EditorPaths::GetEditorAssetsDir().Append("Shaders").
                         Append("Include"));
    return lookUpPaths;
}

