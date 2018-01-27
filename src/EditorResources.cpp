#include "BangEditor/EditorResources.h"

#include "BangEditor/EditorPaths.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorResources::EditorResources()
{
}

EditorResources::~EditorResources()
{
}

Array<Path> EditorResources::GetLookUpPaths() const
{
    return {EditorPaths::GetProjectAssetsDir(), Paths::GetEngineAssetsDir()};
}

