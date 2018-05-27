#include "BangEditor/EditorResources.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorMeshFactory.h"
#include "BangEditor/MaterialPreviewFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorResources::EditorResources()
{
    m_materialPreviewFactory = new MaterialPreviewFactory();
}

EditorResources::~EditorResources()
{
    delete m_materialPreviewFactory;
}

MaterialPreviewFactory *EditorResources::GetMaterialPreviewFactory() const
{
    return m_materialPreviewFactory;
}

EditorResources *EditorResources::GetInstance()
{
    return DCAST<EditorResources*>( Resources::GetInstance() );
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
    return lookUpPaths;
}

