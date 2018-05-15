#include "BangEditor/Hierarchy.h"

#include <functional>

#include "Bang/Input.h"
#include "Bang/Timer.h"
#include "Bang/Scene.h"
#include "Bang/Dialog.h"
#include "Bang/Prefab.h"
#include "Bang/UIList.h"
#include "Bang/UITree.h"
#include "Bang/UICanvas.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/UIRendererCacher.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/Explorer.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/UIContextMenu.h"
#include "BangEditor/HierarchyItem.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Hierarchy::Hierarchy()
{
    SetName("Hierarchy");

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinWidth(200);
    le->SetPreferredWidth(400);
    le->SetFlexibleSize( Vector2::One );

    GameObjectFactory::CreateUIGameObjectInto(this);
    UIRendererCacher *rendCacher = GameObjectFactory::CreateUIRendererCacherInto(this);
    GameObject *rendererCacherContainer = rendCacher->GetContainer();
    rendCacher->GetContainer()->GetRectTransform()->SetMargins(0, 5, 0, 5);

    p_tree = GameObjectFactory::CreateUITree();
    GetUITree()->GetUIList()->GetScrollPanel()->SetForceHorizontalFit(true);

    UIScrollPanel *scrollPanel = GetUITree()->GetUIList()->GetScrollPanel();
    scrollPanel->SetVerticalScrollBarSide(HorizontalSide::Left);
    scrollPanel->GetScrollArea()->GetBackground()->SetTint(Color::Zero);
    scrollPanel->SetVerticalShowScrollMode(ShowScrollMode::Always);
    scrollPanel->SetHorizontalShowScrollMode(ShowScrollMode::Always);
    scrollPanel->SetVerticalScrollEnabled(true);
    scrollPanel->SetHorizontalScrollEnabled(true);

    GameObject *treeGo = GetUITree()->GetGameObject();
    UILayoutElement *treeLE = treeGo->AddComponent<UILayoutElement>();
    treeLE->SetFlexibleSize( Vector2::One );

    GetUITree()->SetSelectionCallback(
                          [this](GOItem *item, UIList::Action action)
                          { this->TreeSelectionCallback(item, action); } );
    GetUITree()->EventEmitter<IUITreeListener>::RegisterListener(this);
    treeGo->SetParent(rendererCacherContainer);

    Editor::GetInstance()->EventEmitter<IEditorListener>::RegisterListener(this);
    EditorSceneManager::GetActive()->
            EventEmitter<ISceneManagerListener>::RegisterListener(this);

    UIFocusable *focusable = AddComponent<UIFocusable>();
    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback([this](MenuItem *menuRootItem)
    {
        OnCreateContextMenu(menuRootItem);
    });
    p_contextMenu->AddButtonPart(this);

    ShortcutManager::RegisterShortcut(Shortcut(Key::F2, "Rename"),
                                      &Hierarchy::OnShortcutPressed);
    ShortcutManager::RegisterShortcut(Shortcut(Key::LCtrl, Key::C, "Copy"),
                                      &Hierarchy::OnShortcutPressed);
    ShortcutManager::RegisterShortcut(Shortcut(Key::LCtrl, Key::X, "Cut"),
                                      &Hierarchy::OnShortcutPressed);
    ShortcutManager::RegisterShortcut(Shortcut(Key::LCtrl, Key::V, "Paste"),
                                      &Hierarchy::OnShortcutPressed);
    ShortcutManager::RegisterShortcut(Shortcut(Key::LCtrl, Key::D, "Duplicate"),
                                      &Hierarchy::OnShortcutPressed);
    ShortcutManager::RegisterShortcut(Shortcut(Key::Delete, "Delete"),
                                      &Hierarchy::OnShortcutPressed);
}

Hierarchy::~Hierarchy()
{
}

void Hierarchy::Update()
{
    GameObject::Update();
}

void Hierarchy::OnChildAdded(GameObject *addedChild, GameObject*)
{
    bool isScene = (DCAST<Scene*>(addedChild));
    Scene *goScene = addedChild->GetScene();
    Scene *openScene = EditorSceneManager::GetOpenScene();
    if (!isScene && goScene && goScene == openScene)
    {
        AddGameObject(addedChild);
    }
}

void Hierarchy::OnChildRemoved(GameObject *removedChild, GameObject*)
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
        if (!selectedHItem) { return; }

        GetUITree()->SetSelection(selectedHItem);
    }
    else
    {
        GetUITree()->GetUIList()->ClearSelection();
    }
}

void Hierarchy::OnCreateEmpty(HierarchyItem *item)
{
    GameObject *empty = GameObjectFactory::CreateGameObjectNamed("Empty");

    GameObject *parent = nullptr;
    if (item) { parent = item->GetReferencedGameObject(); }
    else { parent = EditorSceneManager::GetOpenScene(); }

    if (parent) { empty->SetParent(parent); }
}

void Hierarchy::OnRename(HierarchyItem *item)
{
    GameObject *go = item->GetReferencedGameObject();
    String previousName = go->GetName();
    String newName = Dialog::GetString("Rename", "Introduce the new name:",
                                       previousName);
    if (!newName.IsEmpty()) { go->SetName(newName); }
}

void Hierarchy::OnRemove(HierarchyItem *item)
{
    GameObject::Destroy( item->GetReferencedGameObject() );
}

void Hierarchy::OnCopy(HierarchyItem *item)
{
    EditorClipboard::CopyGameObject( item->GetReferencedGameObject() );
}

void Hierarchy::OnCut(HierarchyItem *item)
{
    EditorClipboard::CopyGameObject( item->GetReferencedGameObject() );
    GameObject::Destroy(item->GetReferencedGameObject());
}

void Hierarchy::OnPaste(HierarchyItem *item)
{
    if (!EditorClipboard::HasCopiedGameObject()) { return; }

    bool pastingDirectlyOverInspector = !item;
    GameObject *pastingOverGo = item ? item->GetReferencedGameObject() :
                                       EditorSceneManager::GetOpenScene();
    if (!pastingOverGo) { return; }

    int pastingOverIndex = pastingDirectlyOverInspector ?
                 pastingOverGo->GetChildren().Size() :
                (pastingOverGo->GetChildren().IndexOf(pastingOverGo + 1));

    GameObject *original = EditorClipboard::GetCopiedGameObject();
    GameObject *clone = original->Clone();
    clone->SetName( GameObjectFactory::GetGameObjectDuplicateName(original) );
    clone->SetParent(pastingOverGo, pastingOverIndex);

    // Auto-select
    Editor::SelectGameObject(clone);
}

void Hierarchy::OnDuplicate(HierarchyItem *item)
{
    OnCopy(item);
    OnPaste( SCAST<HierarchyItem*>(GetUITree()->GetParentItem(item)) );
}

void Hierarchy::OnCreatePrefab(HierarchyItem *item)
{
    RH<Prefab> prefabRH = Resources::Create<Prefab>();
    prefabRH.Get()->SetGameObject( item->GetReferencedGameObject() );

    Path exportFilepath = Explorer::GetInstance()->GetCurrentPath().
                          Append(item->GetReferencedGameObject()->GetName()).
                          AppendExtension(Extensions::GetPrefabExtension()).
                          GetDuplicatePath();
    Resources::CreateResourceXMLAndImportFile(prefabRH.Get(), exportFilepath);

    Explorer::GetInstance()->ForceCheckFileChanges();
}

void Hierarchy::OnItemMoved(GameObject *item,
                            GameObject *oldParentItem, int oldIndexInsideParent,
                            GameObject *newParentItem, int newIndexInsideParent)
{
    IUITreeListener::OnItemMoved(item, oldParentItem, oldIndexInsideParent,
                                 newParentItem, newIndexInsideParent);

    SetReceiveEvents(false);
    GameObject *movedGo = GetGameObjectFromItem(item);
    GameObject *newParent = GetGameObjectFromItem(newParentItem);
    if (!newParent) { newParent = movedGo->GetScene(); }
    movedGo->SetParent(newParent, newIndexInsideParent);
    SetReceiveEvents(true);
}

void Hierarchy::OnCreateContextMenu(MenuItem *menuRootItem)
{
    menuRootItem->SetFontSize(12);

    MenuItem *createEmpty = menuRootItem->AddItem("Create Empty");
    createEmpty->SetSelectedCallback([this](MenuItem*)
    {
        OnCreateEmpty(nullptr);
    });

    menuRootItem->AddSeparator();

    MenuItem *paste = menuRootItem->AddItem("Paste");
    paste->SetSelectedCallback([this](MenuItem*) { OnPaste(nullptr); });
    paste->SetOverAndActionEnabled( EditorClipboard::HasCopiedGameObject() );
}

void Hierarchy::OnSceneLoaded(Scene *scene, const Path&)
{
    Clear();
    if (scene)
    {
        const List<GameObject*> &sceneGos = scene->GetChildren();
        for (GameObject *go : sceneGos)
        {
            AddGameObject(go);
        }
        scene->EventEmitter<IChildrenListener>::RegisterListener(this);
    }
}

void Hierarchy::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    GameObject *destroyedGo = DCAST<GameObject*>(object);
    if (destroyedGo) { RemoveGameObject(destroyedGo); }
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
        !go->GetComponentInParent<HideInHierarchy>(true))
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

        go->EventEmitter<IChildrenListener>::RegisterListener(this);
        go->EventEmitter<IDestroyListener>::RegisterListener(this);
        goItem->EventEmitter<IHierarchyItemListener>::RegisterListener(this);
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
    HierarchyItem *hItem = SCAST<HierarchyItem*>(item);
    return hItem->GetReferencedGameObject();
}

void Hierarchy::OnShortcutPressed(const Shortcut &shortcut)
{
    Hierarchy *h = Hierarchy::GetInstance();

    HierarchyItem *selectedItem =
            h->GetItemFromGameObject( h->GetSelectedGameObject() );
    if (selectedItem)
    {
        if (shortcut.GetName() == "Rename")
        { selectedItem->Rename(); }

        if (shortcut.GetName() == "Copy")
        { selectedItem->Copy(); }

        if (shortcut.GetName() == "Cut")
        { selectedItem->Cut(); }

        if (shortcut.GetName() == "Paste")
        { selectedItem->Paste(); }

        if (shortcut.GetName() == "Duplicate")
        { selectedItem->Duplicate(); }

        if (shortcut.GetName() == "Delete")
        { selectedItem->Remove(); }
    }
}

UITree *Hierarchy::GetUITree() const { return p_tree; }
