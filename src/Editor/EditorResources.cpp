#include "BangEditor/EditorResources.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorMeshFactory.h"
#include "BangEditor/ModelPreviewFactory.h"
#include "BangEditor/MaterialPreviewFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorResources::EditorResources()
{
    m_materialPreviewFactory = new MaterialPreviewFactory();
    m_modelPreviewFactory = new ModelPreviewFactory();
}

EditorResources::~EditorResources()
{
    delete m_modelPreviewFactory;
    delete m_materialPreviewFactory;
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
    m_modelPreviewFactory->Init();
    m_materialPreviewFactory->Init();
}

MeshFactory *EditorResources::CreateMeshFactory() const
{
    return new EditorMeshFactory();
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

