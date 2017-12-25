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
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Hierarchy::Hierarchy() : EditorUITab("Hierarchy")
{
    UILayoutElement *le = GetComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(100) );
    le->SetPreferredSize( Vector2i(150, 200) );

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
    treeGo->SetParent(this);

    ObjectManager::RegisterCreateListener(this);
    Editor::RegisterListener<IEditorSelectionListener>(this);
    EditorSceneManager::GetEditorScene()->
            EventEmitter<IEditorOpenSceneListener>::RegisterListener(this);
}

Hierarchy::~Hierarchy()
{
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
    GameObject *go = Cast<GameObject*>(object);
    if (go) { go->EventEmitter<IChildrenListener>::RegisterListener(this); }
}

void Hierarchy::OnChildAdded(GameObject *addedChild, GameObject *parent)
{
    bool isScene = (DCAST<Scene*>(addedChild));
    Scene *goScene = addedChild->GetScene();
    Scene *openScene = EditorSceneManager::GetOpenScene();
    if (!isScene && goScene && goScene == openScene)
    {
        AddGameObject(addedChild);
    }
}

void Hierarchy::OnChildRemoved(GameObject *removedChild, GameObject *parent)
{
    if (m_gameObjectToItem.ContainsKey(removedChild))
    {
        RemoveGameObject(removedChild);
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

void Hierarchy::OnOpenScene(Scene *scene)
{
    Clear();
    if (scene)
    {
        const List<GameObject*> &sceneGos = scene->GetChildren();
        for (GameObject *go : sceneGos)
        {
            AddGameObject(go);
        }
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
    if (!go->GetComponent<HideInHierarchy>() &&
        !go->GetComponentInParent<HideInHierarchy>())
    {
        bool topItem = (go->GetScene() == go->GetParent());

        HierarchyItem *goItem = GameObject::Create<HierarchyItem>();
        goItem->SetReferencedGameObject( go );

        // Get index inside parent, without counting hidden ones
        HierarchyItem *parentItem = GetItemFromGameObject(go->GetParent());
        int indexInsideParent = go->GetParent()->GetChildren().IndexOf(go);
        for (GameObject *child : go->GetParent()->GetChildren())
        {
            if (child->HasComponent<HideInHierarchy>()) { --indexInsideParent; }
        }

        if (indexInsideParent >= 0)
        {
            GetUITree()->AddItem(goItem, topItem ? nullptr : parentItem,
                                 indexInsideParent);
            m_gameObjectToItem.Add(go, goItem);

            for (GameObject *child : go->GetChildren()) // Add children too
            {
                AddGameObject(child);
            }
        }
    }
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
