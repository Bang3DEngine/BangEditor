#include "BangEditor/PrefabExplorerItem.h"

#include "Bang/GameObject.h"
#include "Bang/Prefab.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
#include "Bang/Scene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/MenuItem.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

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
    createGo->SetSelectedCallback([this](MenuItem*)
    {
        Scene *openScene = EditorSceneManager::GetOpenScene();
        if (openScene)
        {
            RH<Prefab> prefabRH = Resources::Load<Prefab>(GetPath());
            if (prefabRH)
            {
                GameObject *gameObject = prefabRH.Get()->Instantiate();
                gameObject->SetParent(openScene);
            }
        }
    });
}

