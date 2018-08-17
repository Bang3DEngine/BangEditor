#include "BangEditor/Hierarchy.h"

#include <functional>

#include "Bang/Input.h"
#include "Bang/Timer.h"
#include "Bang/Model.h"
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
#include "Bang/UIDragDroppable.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/UIRendererCacher.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/Explorer.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/ExplorerItem.h"
#include "BangEditor/UIContextMenu.h"
#include "BangEditor/HierarchyItem.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/UndoRedoMoveGameObject.h"
#include "BangEditor/UndoRedoCreateGameObject.h"
#include "BangEditor/UndoRedoRemoveGameObject.h"

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
    rendCacher->SetCachingEnabled(false);

    p_tree = GameObjectFactory::CreateUITree();
    GetUITree()->GetUIList()->GetScrollPanel()->SetForceHorizontalFit(true);

    UIScrollPanel *scrollPanel = GetUITree()->GetUIList()->GetScrollPanel();
    scrollPanel->SetVerticalScrollBarSide(HorizontalSide::LEFT);
    scrollPanel->GetScrollArea()->GetBackground()->SetTint(Color::Zero);
    scrollPanel->SetVerticalShowScrollMode(ShowScrollMode::ALWAYS);
    scrollPanel->SetHorizontalShowScrollMode(ShowScrollMode::ALWAYS);
    scrollPanel->SetVerticalScrollEnabled(true);
    scrollPanel->SetHorizontalScrollEnabled(true);

    GameObject *treeGo = GetUITree()->GetGameObject();
    UILayoutElement *treeLE = treeGo->AddComponent<UILayoutElement>();
    treeLE->SetFlexibleSize( Vector2::One );

    GetUITree()->SetSelectionCallback(
                          [this](GOItem *item, UIList::Action action)
                          { this->TreeSelectionCallback(item, action); } );
    GetUITree()->EventEmitter<IEventsUITree>::RegisterListener(this);
    treeGo->SetParent(rendererCacherContainer);

    Editor::GetInstance()->EventEmitter<IEventsEditor>::RegisterListener(this);
    EditorSceneManager::GetActive()->
            EventEmitter<IEventsSceneManager>::RegisterListener(this);

    UIFocusable *focusable = AddComponent<UIFocusable>();
    focusable->AddEventCallback([](IFocusable*,
                                   const UIEvent &event)
    {
        if (event.type == UIEvent::Type::MOUSE_CLICK_DOWN)
        {
            Editor::SelectGameObject(nullptr);
            return UIEventResult::INTERCEPT;
        }
        return UIEventResult::IGNORE;
    });

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback([this](MenuItem *menuRootItem)
    {
        OnCreateContextMenu(menuRootItem);
    });
    p_contextMenu->AddButtonPart(this);

    ShortcutManager::RegisterShortcut(Shortcut(Key::F2, "Rename"),
                                      &Hierarchy::OnShortcutPressed);
    ShortcutManager::RegisterShortcut(Shortcut(Key::LCTRL, Key::C, "Copy"),
                                      &Hierarchy::OnShortcutPressed);
    ShortcutManager::RegisterShortcut(Shortcut(Key::LCTRL, Key::X, "Cut"),
                                      &Hierarchy::OnShortcutPressed);
    ShortcutManager::RegisterShortcut(Shortcut(Key::LCTRL, Key::V, "Paste"),
                                      &Hierarchy::OnShortcutPressed);
    ShortcutManager::RegisterShortcut(Shortcut(Key::LCTRL, Key::D, "Duplicate"),
                                      &Hierarchy::OnShortcutPressed);
    ShortcutManager::RegisterShortcut(Shortcut(Key::DELETE, "Delete"),
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
        RemoveGameObjectItem( GetItemFromGameObject(removedChild) );
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
    RemoveGameObject( item->GetReferencedGameObject() );
}

void Hierarchy::OnCopy(HierarchyItem *item)
{
    EditorClipboard::CopyGameObject( item->GetReferencedGameObject() );
}

void Hierarchy::OnCut(HierarchyItem *item)
{
    GameObject *cutGo = item->GetReferencedGameObject();
    UndoRedoManager::PushAction( new UndoRedoRemoveGameObject(cutGo) );
    EditorClipboard::CopyGameObject(cutGo);
    cutGo->SetParent(nullptr);
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

    UndoRedoManager::PushAction( new UndoRedoCreateGameObject(clone) );

    // Auto-select
    Editor::SelectGameObject(clone, false);
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

void Hierarchy::OnItemMoved(GOItem *item,
                            GOItem *oldParentItem,
                            int oldIndexInsideParent,
                            GOItem *newParentItem,
                            int newIndexInsideParent)
{
    IEventsUITree::OnItemMoved(item, oldParentItem, oldIndexInsideParent,
                                 newParentItem, newIndexInsideParent);

    GameObject *go = GetGameObjectFromItem(item);
    GameObject *oldParent = GetGameObjectFromItem(oldParentItem);
    GameObject *newParent = GetGameObjectFromItem(newParentItem);
    if (!oldParent) { oldParent = go->GetScene(); }
    if (!newParent) { newParent = go->GetScene(); }

    UndoRedoManager::PushAction(
                new UndoRedoMoveGameObject(go, oldParent, oldIndexInsideParent,
                                           newParent, newIndexInsideParent) );

    IEventListenerCommon::SetReceiveEventsCommon(false);
    GameObject *movedGo = GetGameObjectFromItem(item);
    if (!newParent) { newParent = movedGo->GetScene(); }
    movedGo->SetParent(newParent, newIndexInsideParent);
    Editor::SelectGameObject(movedGo, false);
    IEventListenerCommon::SetReceiveEventsCommon(true);
}

void Hierarchy::OnDropOutside(GOItem *item)
{
    if (UICanvas::GetActive(this)->IsMouseOver(this, true))
    {
        GameObject *go = GetGameObjectFromItem(item);
        go->SetParent( EditorSceneManager::GetOpenScene(), -1 );
    }
}

void Hierarchy::OnDropFromOutside(UIDragDroppable *dropped,
                                  GameObject *newParentItem,
                                  int newIndexInsideParent)
{
    if (ExplorerItem *expItem = DCAST<ExplorerItem*>(dropped->GetGameObject()))
    {
        if ( Extensions::Equals(expItem->GetPath().GetExtension(),
                                Extensions::GetModelExtensions()) )
        {
            RH<Model> modelRH = Resources::Load<Model>( expItem->GetPath() );
            if (Model *model = modelRH.Get())
            {
                GameObject *newParent = (newParentItem ?
                                             GetGameObjectFromItem(newParentItem) :
                                             EditorSceneManager::GetOpenScene());
                GameObject *modelGo = model->CreateGameObjectFromModel();
                modelGo->SetParent(newParent, newIndexInsideParent);

                UndoRedoManager::PushAction(
                                       new UndoRedoCreateGameObject(modelGo) );
            }
        }
    }
}

bool Hierarchy::AcceptDragOrDrop(UIDragDroppable *dd)
{
    if ( UITreeItemContainer *treeItemCont = DCAST<UITreeItemContainer*>(
                                                        dd->GetGameObject()) )
    {
        if (GameObject *containedItem = treeItemCont->GetContainedItem())
        {
            if (DCAST<HierarchyItem*>(containedItem))
            {
                return true;
            }
        }
    }

    if (ExplorerItem *expItem = DCAST<ExplorerItem*>(dd->GetGameObject()))
    {
        return Extensions::Equals(expItem->GetPath().GetExtension(),
                                  Extensions::GetModelExtensions());
    }

    return false;
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
        scene->EventEmitter<IEventsChildren>::RegisterListener(this);
    }
}

void Hierarchy::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    GameObject *destroyedGo = DCAST<GameObject*>(object);
    if (destroyedGo) { RemoveGameObjectItem( GetItemFromGameObject(destroyedGo) ); }
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

        go->EventEmitter<IEventsChildren>::RegisterListener(this);
        go->EventEmitter<IEventsDestroy>::RegisterListener(this);
        goItem->EventEmitter<IEventsHierarchyItem>::RegisterListener(this);

        GetUITree()->SetItemCollapsed(goItem, true);
    }
}

void Hierarchy::RemoveGameObject(GameObject *go)
{
    UndoRedoManager::PushAction( new UndoRedoRemoveGameObject(go) );

    // Fake remove to let undo/redo keep the reference to it...
    go->SetParent(nullptr);
}

void Hierarchy::RemoveGameObjectItem(HierarchyItem *item)
{
    if (item)
    {
        GameObject *go = item->GetReferencedGameObject();
        GetUITree()->RemoveItem(item);
        m_gameObjectToItem.Remove(go);
        Editor::SelectGameObject(nullptr, false);
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
