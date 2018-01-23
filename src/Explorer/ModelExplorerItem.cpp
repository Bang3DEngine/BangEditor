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
            RH<Model> model = Resources::Load<Model>(GetPath());
            if (model)
            {
                GameObject *gameObject = model.Get()->CreateGameObjectFromModel();
                gameObject->SetParent(openScene);
            }
        }
    });
}

