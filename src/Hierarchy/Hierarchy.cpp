#include "BangEditor/Hierarchy.h"

#include <functional>

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

#include "BangEditor/EditorScene.h"
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

    GetUITree()->SetSelectionCallback(
                          [this](GOItem *item, UIList::Action action)
                          { this->TreeSelectionCallback(item, action); } );

    SetAsChild(treeGo);
}

Hierarchy::~Hierarchy()
{

}

void Hierarchy::OnStart()
{
    Editor::RegisterListener<IEditorSelectionListener>(this);
}

void Hierarchy::OnGameObjectSelected(GameObject *selectedGameObject)
{
    HierarchyItem *selectedHItem = GetItemFromGameObject(selectedGameObject);
    ENSURE(selectedHItem);

    GOItem *selectedHItemContainer = selectedHItem->GetParent();
    GetUITree()->GetUIList()->SetSelection(selectedHItemContainer);
}

void Hierarchy::Clear()
{
    GetUITree()->Clear();
    m_gameObjectToItem.Clear();
}

Hierarchy *Hierarchy::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetHierarchy();
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

void Hierarchy::UpdateItemFromGameObject(GOItem *parentItem,
                                         GameObject *parentGo)
{
    // Compare the hierarchy tree and the scene implicit tree, and find the
    // differences.
    List<GOItem*> itemChildren = GetUITree()->GetChildrenItems(parentItem);
    const List<GameObject*>& goChildren = parentGo->GetChildren();

    // Two lists to keep track of which things to add and remove after diff
    List<GameObject*> gameObjectsToAdd;
    List<HierarchyItem*> itemsToRemove;

    // Begin the comparison iterating through both lists
    auto childGoIt = goChildren.Begin();
    auto childItemIt = itemChildren.Begin();
    while (childGoIt != goChildren.End() && childItemIt != itemChildren.End())
    {
        GameObject *childGo = *childGoIt;
        HierarchyItem *childItem = SCAST<HierarchyItem*>(*childItemIt);
        if (childGo != childItem->GetReferencedGameObject())
        {
            gameObjectsToAdd.PushBack(childGo);
            itemsToRemove.PushBack(childItem);
        }
        else
        {
            // No difference, so just update
            UpdateItemFromGameObject(childItem, childGo);
        }
        ++childGoIt;
        ++childItemIt;
    }

    // Consume both iterators if they are not already ended
    while (childGoIt != goChildren.End())
    {
        gameObjectsToAdd.PushBack(*childGoIt);
        ++childGoIt;
    }

    while (childItemIt != itemChildren.End())
    {
        itemsToRemove.PushBack( SCAST<HierarchyItem*>(*childItemIt) );
        ++childItemIt;
    }

    // Now that we have the diff, add the new gameObject items, and remove the
    // outdated ones
    for (HierarchyItem *itemToRemove : itemsToRemove) // Remove
    {
        GetUITree()->RemoveItem(itemToRemove);
    }

    for (GameObject *goToAdd : gameObjectsToAdd) // Add
    {
        AddGameObjectItemToHierarchy(goToAdd, true);
    }
}

void Hierarchy::TreeSelectionCallback(GOItem *item, UIList::Action action)
{
    HierarchyItem *hItem = SCAST<HierarchyItem*>(item);
    hItem->OnSelectionCallback(action);
}

void Hierarchy::AddGameObjectItemToHierarchy(GameObject *go, bool topItem)
{
    HierarchyItem *goItem = GameObject::Create<HierarchyItem>();
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
