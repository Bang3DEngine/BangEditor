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

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ExplorerItem *ExplorerItemFactory::CreateExplorerItem(const Path &path)
{
    ExplorerItem *explorerItem = nullptr;
    if (path.HasExtension(Extensions::GetModelExtensions()))
    {
        explorerItem = GameObject::Create<ModelExplorerItem>();
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
            const Array< RH<Mesh> > &meshes = model.Get()->GetMeshes();
            const Array< RH<Material> > &materials = model.Get()->GetMaterials();
            int i = 0;
            for (RH<Mesh> mesh : meshes)
            {
                String meshName = "Mesh_" + String(i++);
                Path meshPath = path.Append(meshName);
                ExplorerItem *meshExplorerItem =
                        ExplorerItemFactory::CreateExplorerItem(meshPath);
                children.PushBack( meshExplorerItem );
            }
            i = 0;
            for (RH<Material> material : materials)
            {
                String materialName = "Material_" + String(i++);
                Path materialPath = path.Append(materialName);
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

