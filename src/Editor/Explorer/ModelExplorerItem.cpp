#include "BangEditor/ModelExplorerItem.h"

#include "Bang/GameObject.h"
#include "Bang/Model.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
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
            RH<Model> model = Resources::Load<Model>(GetPath());
            if (model)
            {
                GameObject *gameObject =
                    model.Get()->CreateGameObjectFromModel();
                gameObject->SetParent(openScene);
            }
        }
    });
}
