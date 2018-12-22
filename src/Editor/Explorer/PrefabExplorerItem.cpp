#include "BangEditor/PrefabExplorerItem.h"

#include "Bang/GameObject.h"
#include "Bang/Prefab.h"
#include "Bang/AssetHandle.h"
#include "Bang/Assets.h"
#include "Bang/Assets.tcc"
#include "Bang/Scene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/MenuItem.h"

using namespace Bang;
using namespace BangEditor;

PrefabExplorerItem::PrefabExplorerItem()
{
}

PrefabExplorerItem::~PrefabExplorerItem()
{
}

void PrefabExplorerItem::OnCreateContextMenu(MenuItem *menuRootItem)
{
    ExplorerItem::OnCreateContextMenu(menuRootItem);

    MenuItem *createGo = menuRootItem->AddItem("Create GameObject from prefab");
    createGo->SetSelectedCallback([this](MenuItem *) {
        Scene *openScene = EditorSceneManager::GetOpenScene();
        if (openScene)
        {
            AH<Prefab> prefabAH = Assets::Load<Prefab>(GetPath());
            if (prefabAH)
            {
                GameObject *gameObject = prefabAH.Get()->Instantiate();
                gameObject->SetParent(openScene);
            }
        }
    });
}
