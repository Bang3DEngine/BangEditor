#include "BangEditor/Hierarchy.h"

#include <functional>

#include "Bang/Input.h"
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

#include "Bang/Material.h"
Hierarchy::Hierarchy() : EditorUITab("Hierarchy")
{
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

    ObjectManager::RegisterCreateListener(this);
    ObjectManager::RegisterDestroyListener(this);
}

Hierarchy::~Hierarchy()
{
}

void Hierarchy::ToggleItemCollapsed(HierarchyItem *item)
{
    SetItemCollapsed(item, !p_tree->IsItemCollapsed(item));
}

void Hierarchy::SetItemCollapsed(HierarchyItem *item, bool collapsed)
{
    p_tree->SetItemCollapsed(item, collapsed);
}

void Hierarchy::OnStart()
{
    Editor::RegisterListener<IEditorSelectionListener>(this);
}

void Hierarchy::Update()
{
    EditorUITab::Update();
    if (Input::GetKeyDownRepeat(Key::Delete))
    {
        GameObject *selectedGameObject = GetSelectedGameObject();
        if (selectedGameObject) { GameObject::Destroy(selectedGameObject); }
    }
}

void Hierarchy::OnCreated(Object *object)
{
    OnCreatedDestroyed(object, true);
}
void Hierarchy::OnDestroyed(Object *object)
{
    OnCreatedDestroyed(object, false);
}
void Hierarchy::OnCreatedDestroyed(Object *object, bool created)
{
    GameObject *go = Cast<GameObject*>(object);
    if (go)
    {
        bool isScene = (DCAST<Scene*>(go));
        Scene *goScene = go->GetScene();
        Scene *editorScene = SceneManager::GetActiveScene();
        if (!isScene && goScene && goScene != editorScene && go != editorScene)
        {
            if (created) { AddGameObject(go); }
            else { RemoveGameObject(go); }
        }
    }
}

GOItem *Hierarchy::GetSelectedGameObject() const
{
    GOItem *selectedItem = GetSelectedItem();
    return selectedItem ? GetGameObjectFromItem(selectedItem) : nullptr;
}

GOItem *Hierarchy::GetSelectedItem() const
{
    return GetUITree()->GetSelectedItem();
}

void Hierarchy::OnGameObjectSelected(GameObject *selectedGameObject)
{
    if (selectedGameObject)
    {
        HierarchyItem *selectedHItem = GetItemFromGameObject(selectedGameObject);
        ENSURE(selectedHItem);

        GetUITree()->SetSelection(selectedHItem);
    }
    else
    {
        GetUITree()->GetUIList()->ClearSelection();
    }
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

void Hierarchy::TreeSelectionCallback(GOItem *item, UIList::Action action)
{
    HierarchyItem *hItem = Cast<HierarchyItem*>(item);
    hItem->OnSelectionCallback(action);
}

void Hierarchy::AddGameObject(GameObject *go)
{
    bool topItem = (go->GetScene() == go->GetParent());

    HierarchyItem *goItem = GameObject::Create<HierarchyItem>();
    goItem->SetReferencedGameObject( go );

    HierarchyItem *parentItem = GetItemFromGameObject(go->GetParent());
    GetUITree()->AddItem(goItem, topItem ? nullptr : parentItem);
    m_gameObjectToItem.Add(go, goItem);
}

void Hierarchy::RemoveGameObject(GameObject *go)
{
    HierarchyItem *goItem = GetItemFromGameObject(go);
    if (goItem)
    {
        GetUITree()->RemoveItem(goItem);
        m_gameObjectToItem.Remove(go);
    }
}

HierarchyItem* Hierarchy::GetItemFromGameObject(GameObject *go) const
{
    if (m_gameObjectToItem.ContainsKey(go))
    {
        return m_gameObjectToItem.Get(go);
    }
    return nullptr;
}

GameObject *Hierarchy::GetGameObjectFromItem(GOItem *item) const
{
    if (!item) { return nullptr; }
    HierarchyItem *hItem = Cast<HierarchyItem*>(item);
    return hItem->GetReferencedGameObject();
}

UITree *Hierarchy::GetUITree() const { return p_tree; }
