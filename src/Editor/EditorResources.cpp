#include "BangEditor/EditorResources.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorMeshFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorResources::EditorResources()
{
}

EditorResources::~EditorResources()
{
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

