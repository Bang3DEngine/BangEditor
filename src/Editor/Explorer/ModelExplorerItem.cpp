#include "BangEditor/ModelExplorerItem.h"

#include "Bang/GameObject.h"
#include "Bang/Model.h"
#include "Bang/AssetHandle.h"
#include "Bang/Assets.h"
#include "Bang/Assets.tcc"
#include "Bang/Scene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/MenuItem.h"

using namespace Bang;
using namespace BangEditor;

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
    createGo->SetSelectedCallback([this](MenuItem *) {
        Scene *openScene = EditorSceneManager::GetOpenScene();
        if (openScene)
        {
            AH<Model> model = Assets::Load<Model>(GetPath());
            if (model)
            {
                GameObject *gameObject =
                    model.Get()->CreateGameObjectFromModel();
                gameObject->SetParent(openScene);
            }
        }
    });
}
