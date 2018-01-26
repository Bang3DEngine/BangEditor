#include "BangEditor/ExplorerItemFactory.h"

#include "Bang/Mesh.h"
#include "Bang/Path.h"
#include "Bang/Model.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/ResourceHandle.h"

#include "BangEditor/ExplorerItem.h"
#include "BangEditor/ModelExplorerItem.h"
#include "BangEditor/PrefabExplorerItem.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ExplorerItem *ExplorerItemFactory::CreateExplorerItem(const Path &path)
{
    ExplorerItem *explorerItem = nullptr;
    if (path.HasExtension(Extensions::GetModelExtensions()))
    {
        explorerItem = GameObject::Create<ModelExplorerItem>();
    }
    else if (path.HasExtension(Extensions::GetPrefabExtension()))
    {
        explorerItem = GameObject::Create<PrefabExplorerItem>();
    }
    else
    {
        explorerItem = GameObject::Create<ExplorerItem>();
    }

    explorerItem->SetPath(path);

    return explorerItem;
}

List<ExplorerItem *> ExplorerItemFactory::CreateAndGetChildrenExplorerItems(
                                                const Path &path)
{
    List<ExplorerItem*> children;

    if (path.IsDir())
    {
        List<Path> subPaths = path.GetSubPaths(Path::FindFlag::Simple);
        Paths::SortPathsByExtension(&subPaths);
        Paths::SortPathsByName(&subPaths);
        for (const Path &subPath : subPaths)
        {
            ExplorerItem *childExpItem =
                            ExplorerItemFactory::CreateExplorerItem(subPath);
            children.PushBack(childExpItem);
        }
    }
    else if (path.IsFile())
    {
        if (path.HasExtension(Extensions::GetModelExtensions()))
        {
            RH<Model> model = Resources::Load<Model>(path);
            Resources::SetPermanent(model.Get(), true);

            for (const String& meshName : model.Get()->GetMeshesNames())
            {
                Path meshPath = path.Append(meshName).AppendExtension("bmesh");
                ExplorerItem *meshExplorerItem =
                        ExplorerItemFactory::CreateExplorerItem(meshPath);
                children.PushBack( meshExplorerItem );
            }

            for (const String& materialName : model.Get()->GetMaterialsNames())
            {
                Path materialPath = path.Append(materialName).
                        AppendExtension(Extensions::GetMaterialExtension());
                ExplorerItem *materialExplorerItem =
                        ExplorerItemFactory::CreateExplorerItem(materialPath);
                children.PushBack( materialExplorerItem );
            }
        }
    }

    return children;
}

bool ExplorerItemFactory::CanHaveChildren(const Path &path)
{
    return path.IsDir() ||
           path.HasExtension(Extensions::GetModelExtensions());
}

ExplorerItemFactory::ExplorerItemFactory()
{
}

ExplorerItemFactory::~ExplorerItemFactory()
{
}

