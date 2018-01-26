#include "BangEditor/PrefabExplorerItem.h"

#include "Bang/Mesh.h"
#include "Bang/Scene.h"
#include "Bang/Prefab.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/GameObject.h"
#include "Bang/MeshRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

PrefabExplorerItem::PrefabExplorerItem()
{
}

PrefabExplorerItem::~PrefabExplorerItem()
{
}

#include "Bang/Debug.h"
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
            Debug_Peek(prefabRH.Get()->m_gameObjectXMLInfoContent);

            if (prefabRH)
            {
                GameObject *gameObject = prefabRH.Get()->Instantiate();
                gameObject->SetParent(openScene);
            }
        }
    });
}

