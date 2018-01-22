#include "BangEditor/ModelExplorerItem.h"

#include "Bang/Mesh.h"
#include "Bang/Model.h"
#include "Bang/Scene.h"
#include "Bang/ModelIO.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/GameObject.h"
#include "Bang/MeshRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ModelExplorerItem::ModelExplorerItem()
{
}

ModelExplorerItem::~ModelExplorerItem()
{
}

void ModelExplorerItem::OnCreateContextMenu(MenuItem *menuRootItem)
{
    ExplorerItem::OnCreateContextMenu(menuRootItem);

    MenuItem *createGo = menuRootItem->AddItem("Create GameObject from model");
    createGo->SetSelectedCallback([this](MenuItem*)
    {
        Scene *openScene = EditorSceneManager::GetOpenScene();
        if (openScene)
        {
            GameObject *gameObject = GameObjectFactory::CreateGameObject();

            RH<Model> model = Resources::Load<Model>(GetPath());

            const Array< RH<Mesh> > &meshes = model.Get()->GetMeshes();
            const Array< RH<Material> > &materials = model.Get()->GetMaterials();
            for (int i = 0; i < SCAST<int>(meshes.Size()); ++i)
            {
                MeshRenderer *mr = gameObject->AddComponent<MeshRenderer>();
                mr->SetMesh(meshes[i].Get());
                mr->SetMaterial(materials[i].Get());
            }

            gameObject->SetParent(openScene);
        }
    });
}

