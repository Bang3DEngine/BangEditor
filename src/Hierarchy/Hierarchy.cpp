#include "BangEditor/Hierarchy.h"

#include "Bang/Timer.h"
#include "Bang/Scene.h"
#include "Bang/UIList.h"
#include "Bang/UITree.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/HierarchyItem.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Hierarchy::Hierarchy() : EditorUITab("Hierarchy")
{
    Timer *updateTimer = AddComponent<Timer>();
    updateTimer->SetInterval(1.0f);
    updateTimer->AddCallback( std::bind(&Hierarchy::UpdateFromScene, this) );
    updateTimer->Run();

    UILayoutElement *le = GetComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(100) );
    le->SetPreferredSize( Vector2i(200) );

    p_tree = GameObjectFactory::CreateUITree();
    UIScrollPanel *scrollPanel = GetUITree()->GetUIList()->GetScrollPanel();
    scrollPanel->SetVerticalScrollBarSide(HorizontalSide::Left);
    scrollPanel->GetScrollArea()->GetBackground()->SetTint(Color::Zero);
    scrollPanel->SetVerticalShowScrollMode(ShowScrollMode::WhenNeeded);
    scrollPanel->SetHorizontalShowScrollMode(ShowScrollMode::WhenNeeded);

    GameObject *treeGo = GetUITree()->GetGameObject();
    UILayoutElement *treeLE = treeGo->AddComponent<UILayoutElement>();
    treeLE->SetFlexibleSize( Vector2::One );

    SetAsChild(treeGo);
}

Hierarchy::~Hierarchy()
{

}

void Hierarchy::Clear()
{
    GetUITree()->Clear();
    m_gameObjectToItem.Clear();
}

void Hierarchy::UpdateFromScene()
{
    Scene *openScene = EditorSceneManager::GetOpenScene();
    if (openScene)
    {
        UpdateItemFromGameObject(nullptr, openScene);
    }
    else { Clear(); }
}

void Hierarchy::UpdateItemFromGameObject(GOItem *item, GameObject *gameObject)
{
    // Compare the hierarchy tree and the scene implicit tree, and find the
    // differences.
    List<GOItem*> itemChildren = GetUITree()->GetChildrenItems(item);
    const List<GameObject*>& goChildren = gameObject->GetChildren();

    // Two lists to keep track of which things to add and remove after diff
    List<GameObject*> gameObjectsToAdd;
    List<HierarchyItem*> itemsToRemove;

    // Begin the comparison iterating through both lists
    auto goIt = goChildren.Begin();
    auto itemIt = itemChildren.Begin();
    while (goIt != goChildren.End() && itemIt != itemChildren.End())
    {
        GameObject *go = *goIt;
        HierarchyItem *item = SCAST<HierarchyItem*>(*itemIt);
        if (go != item->GetReferencedGameObject())
        {
            gameObjectsToAdd.PushBack(go);
            itemsToRemove.PushBack(item);
        }
        else
        {
            // No difference, so just update
            UpdateItemFromGameObject(item, gameObject);
        }
    }

    // Consume both iterators if they are not already ended
    while (goIt != goChildren.End())
    {
        gameObjectsToAdd.PushBack(*goIt);
    }

    while (itemIt != itemChildren.End())
    {
        itemsToRemove.PushBack( SCAST<HierarchyItem*>(*itemIt) );
    }

    // Now that we have the diff, add the new gameObject items, and remove the
    // outdated ones
    for (GameObject *gameObjectChild : gameObjectsToAdd) // Add
    {
        AddGameObjectItemToHierarchy(gameObjectChild, true);
    }

    for (HierarchyItem *item : itemsToRemove) // Remove
    {
        GetUITree()->RemoveItem(item);
    }
}

void Hierarchy::TreeSelectionCallback(GOItem *item, UIList::Action action)
{
    HierarchyItem *hItem = DCAST<HierarchyItem*>(item); ASSERT(hItem);
    hItem->OnSelectionCallback(action);
}

void Hierarchy::AddGameObjectItemToHierarchy(GameObject *go, bool topItem)
{
    HierarchyItem *goItem = ObjectManager::Create<HierarchyItem>();
    goItem->SetReferencedGameObject( go );

    HierarchyItem *parentItem = GetItemFromGameObject(go->GetParent());
    GetUITree()->AddItem(goItem, topItem ? nullptr : parentItem);
    m_gameObjectToItem.Add(go, goItem);

    const List<GameObject*>& goChildren = go->GetChildren();
    for (GameObject *child : goChildren)
    {
        AddGameObjectItemToHierarchy(child, false);
    }
}

HierarchyItem* Hierarchy::GetItemFromGameObject(GameObject *go)
{
    if (m_gameObjectToItem.ContainsKey(go))
    {
        return m_gameObjectToItem.Get(go);
    }
    return nullptr;
}

UITree *Hierarchy::GetUITree() const { return p_tree; }
