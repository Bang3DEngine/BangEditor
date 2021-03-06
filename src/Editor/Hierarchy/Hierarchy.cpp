#include "BangEditor/Hierarchy.h"

#include <unordered_map>

#include "Bang/Alignment.h"
#include "Bang/Array.h"
#include "Bang/Assert.h"
#include "Bang/AssetHandle.h"
#include "Bang/Assets.h"
#include "Bang/Assets.tcc"
#include "BangMath/Color.h"
#include "Bang/Dialog.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventListenerCommon.h"
#include "Bang/IEventsSceneManager.h"
#include "Bang/IEventsUITree.h"
#include "Bang/Key.h"
#include "Bang/MetaNode.h"
#include "Bang/Model.h"
#include "Bang/Path.h"
#include "Bang/Prefab.h"
#include "Bang/Scene.h"
#include "Bang/UICanvas.h"
#include "Bang/UIDirLayout.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIList.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITree.h"
#include "Bang/UITreeItemContainer.h"
#include "Bang/UMap.tcc"
#include "BangEditor/Editor.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/Explorer.h"
#include "BangEditor/ExplorerItem.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/HierarchyItem.h"
#include "BangEditor/IEventsEditor.h"
#include "BangEditor/IEventsHierarchyItem.h"
#include "BangEditor/MenuBar.h"
#include "BangEditor/MenuItem.h"
#include "BangEditor/UIContextMenu.h"
#include "BangEditor/UndoRedoCreateGameObject.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoMoveGameObject.h"
#include "BangEditor/UndoRedoRemoveGameObject.h"
#include "BangEditor/UndoRedoSerializableChange.h"

namespace Bang
{
class IEventsChildren;
class IEventsDestroy;
}  // namespace Bang

namespace BangEditor
{
class UndoRedoAction;
}

using namespace Bang;
using namespace BangEditor;

Hierarchy::Hierarchy()
{
    SetName("Hierarchy");

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinSize(Vector2i(100));
    le->SetFlexibleSize(Vector2::One());

    GameObjectFactory::CreateUIGameObjectInto(this);

    p_uiTree = GameObjectFactory::CreateUITree();
    GetUITree()->GetUIList()->GetScrollPanel()->SetForceHorizontalFit(true);

    UIScrollPanel *scrollPanel = GetUITree()->GetUIList()->GetScrollPanel();
    scrollPanel->SetVerticalScrollBarSide(HorizontalSide::LEFT);
    scrollPanel->GetScrollArea()->GetBackground()->SetTint(Color::Zero());
    scrollPanel->SetVerticalShowScrollMode(ShowScrollMode::ALWAYS);
    scrollPanel->SetHorizontalShowScrollMode(ShowScrollMode::ALWAYS);
    scrollPanel->SetVerticalScrollEnabled(true);
    scrollPanel->SetHorizontalScrollEnabled(true);

    GameObject *treeGo = GetUITree()->GetGameObject();
    UILayoutElement *treeLE = treeGo->AddComponent<UILayoutElement>();
    treeLE->SetFlexibleSize(Vector2::One());
    GetUITree()->GetUIList()->GetDirLayout()->SetPaddingTop(10);
    GetUITree()
        ->GetUIList()
        ->GetScrollPanel()
        ->GetScrollArea()
        ->GetBackground()
        ->SetTint(Color::White().WithValue(0.7f));

    GetUITree()->SetSelectionCallback(
        [this](GOItem *item, UIList::Action action) {
            this->TreeSelectionCallback(item, action);
        });
    GetUITree()->SetDragDropEnabled(true);
    GetUITree()->EventEmitter<IEventsUITree>::RegisterListener(this);
    treeGo->SetParent(this);

    Editor::GetInstance()->EventEmitter<IEventsEditor>::RegisterListener(this);
    EditorSceneManager::GetActive()
        ->EventEmitter<IEventsSceneManager>::RegisterListener(this);

    UIFocusable *focusable = AddComponent<UIFocusable>();
    focusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback(
        [this](MenuItem *menuRootItem) { OnCreateContextMenu(menuRootItem); });
    p_contextMenu->SetFocusable(focusable);
}

Hierarchy::~Hierarchy()
{
}

void Hierarchy::Update()
{
    GameObject::Update();
}

void Hierarchy::OnChildAdded(GameObject *addedChild, GameObject *)
{
    bool isScene = (DCAST<Scene *>(addedChild));
    Scene *goScene = addedChild->GetScene();
    Scene *openScene = EditorSceneManager::GetOpenScene();
    if (!isScene && goScene && goScene == openScene)
    {
        AddGameObject(addedChild);
    }
}

void Hierarchy::OnChildRemoved(GameObject *removedChild, GameObject *)
{
    if (m_gameObjectToItem.ContainsKey(removedChild))
    {
        RemoveGameObjectItem(GetItemFromGameObject(removedChild));
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
        if (HierarchyItem *selectedHItem =
                GetItemFromGameObject(selectedGameObject))
        {
            GetUITree()->SetSelection(selectedHItem);
        }
    }
    else
    {
        GetUITree()->GetUIList()->ClearSelection();
    }
}

void Hierarchy::DuplicateSelectedItem()
{
    if (GOItem *selectedItem = GetSelectedItem())
    {
        if (HierarchyItem *selectedHItem = DCAST<HierarchyItem *>(selectedItem))
        {
            OnDuplicate(selectedHItem);
        }
    }
}

void Hierarchy::OnRename(HierarchyItem *item)
{
    GameObject *go = item->GetReferencedGameObject();
    String previousName = go->GetName();
    String newName =
        Dialog::GetString("Rename", "Introduce the new name:", previousName);
    if (!newName.IsEmpty())
    {
        go->SetName(newName);
    }
}

void Hierarchy::OnRemove(HierarchyItem *item)
{
    RemoveGameObject(item->GetReferencedGameObject());
}

void Hierarchy::OnCopy(HierarchyItem *item)
{
    EditorClipboard::CopyGameObject(item->GetReferencedGameObject());
}

void Hierarchy::OnCut(HierarchyItem *item)
{
    GameObject *cutGo = item->GetReferencedGameObject();
    UndoRedoManager::PushAction(new UndoRedoRemoveGameObject(cutGo));
    EditorClipboard::CopyGameObject(cutGo);
    cutGo->SetParent(nullptr);
}

void Hierarchy::OnPaste(HierarchyItem *item)
{
    if (!EditorClipboard::HasCopiedGameObject())
    {
        return;
    }

    bool pastingDirectlyOverInspector = !item;
    GameObject *pastingOverGo = item ? item->GetReferencedGameObject()
                                     : EditorSceneManager::GetOpenScene();
    if (pastingOverGo)
    {
        int pastingOverIndex =
            pastingDirectlyOverInspector
                ? pastingOverGo->GetChildren().Size()
                : (pastingOverGo->GetChildren().IndexOf(pastingOverGo + 1));

        GameObject *original = EditorClipboard::GetCopiedGameObject();
        GameObject *clone = original->Clone(false);
        clone->SetName(GameObjectFactory::GetGameObjectDuplicateName(original));

        clone->SetParent(pastingOverGo, pastingOverIndex, true);
        UndoRedoManager::PushAction(new UndoRedoCreateGameObject(clone));

        // Auto-select
        Editor::SelectGameObject(clone, false);
    }
}

void Hierarchy::OnDuplicate(HierarchyItem *item)
{
    OnCopy(item);
    OnPaste(SCAST<HierarchyItem *>(GetUITree()->GetParentItem(item)));
}

void Hierarchy::OnCreatePrefab(HierarchyItem *item)
{
    AH<Prefab> prefabAH = Assets::Create<Prefab>();
    prefabAH.Get()->SetGameObject(item->GetReferencedGameObject());

    Path exportFilepath =
        Explorer::GetInstance()
            ->GetCurrentPath()
            .Append(item->GetReferencedGameObject()->GetName())
            .AppendExtension(Extensions::GetPrefabExtension())
            .GetDuplicatePath();
    Assets::CreateAssetMetaAndImportFile(prefabAH.Get(), exportFilepath);

    Explorer::GetInstance()->ForceCheckFileChanges();
}

void Hierarchy::OnItemMoved(GOItem *item,
                            GOItem *oldParentItem,
                            int oldIndexInsideParent,
                            GOItem *newParentItem,
                            int newIndexInsideParent)
{
    IEventsUITree::OnItemMoved(item,
                               oldParentItem,
                               oldIndexInsideParent,
                               newParentItem,
                               newIndexInsideParent);

    GameObject *go = GetGameObjectFromItem(item);

    GameObject *oldParent = GetGameObjectFromItem(oldParentItem);
    if (!oldParent)
    {
        oldParent = go->GetScene();
    }

    GameObject *newParent = GetGameObjectFromItem(newParentItem);
    if (!newParent)
    {
        newParent = go->GetScene();
    }

    IEventListenerCommon::SetReceiveEventsCommon(false);
    GameObject *movedGo = GetGameObjectFromItem(item);
    if (!newParent)
    {
        newParent = movedGo->GetScene();
    }

    MetaNode prevMeta = go->GetMeta();
    movedGo->SetParent(newParent, newIndexInsideParent, true);
    UndoRedoManager::PushActionsInSameStep(
        {new UndoRedoMoveGameObject(go,
                                    oldParent,
                                    oldIndexInsideParent,
                                    newParent,
                                    newIndexInsideParent),
         new UndoRedoSerializableChange(go, prevMeta, go->GetMeta())});

    Editor::SelectGameObject(movedGo, false);
    IEventListenerCommon::SetReceiveEventsCommon(true);
}

void Hierarchy::OnDropOutside(UIDragDroppable *dropped)
{
    if (!dropped)
    {
        return;
    }

    if (UITreeItemContainer *treeItemCont =
            DCAST<UITreeItemContainer *>(dropped->GetGameObject()))
    {
        HierarchyItem *hItem =
            DCAST<HierarchyItem *>(treeItemCont->GetContainedItem());
        if (UICanvas::GetActive(this)->IsMouseOver(this, true))
        {
            GameObject *go = GetGameObjectFromItem(hItem);
            ASSERT(go);

            GameObject *prevGoParent = go->GetParent();
            int prevIndexInParent = go->GetIndexInsideParent();

            MetaNode prevMeta = go->GetMeta();
            go->SetParent(EditorSceneManager::GetOpenScene(), -1, true);
            UndoRedoManager::PushActionsInSameStep(
                {new UndoRedoMoveGameObject(go,
                                            prevGoParent,
                                            prevIndexInParent,
                                            go->GetParent(),
                                            go->GetIndexInsideParent()),
                 new UndoRedoSerializableChange(go, prevMeta, go->GetMeta())});
        }
    }
    else
    {
        OnDropFromOutside(dropped, nullptr, -1);
    }
}

void Hierarchy::OnDropFromOutside(UIDragDroppable *dropped,
                                  GameObject *newParentItem,
                                  int newIndexInsideParent)
{
    if (!dropped)
    {
        return;
    }

    if (ExplorerItem *expItem = DCAST<ExplorerItem *>(dropped->GetGameObject()))
    {
        if (UICanvas::GetActive(this)->IsMouseOver(this, true))
        {
            if (Extensions::Equals(expItem->GetPath().GetExtension(),
                                   Extensions::GetModelExtensions()))
            {
                AH<Model> modelAH = Assets::Load<Model>(expItem->GetPath());
                if (Model *model = modelAH.Get())
                {
                    GameObject *newParent =
                        (newParentItem ? GetGameObjectFromItem(newParentItem)
                                       : EditorSceneManager::GetOpenScene());
                    GameObject *modelGo = model->CreateGameObjectFromModel();

                    modelGo->SetParent(newParent, newIndexInsideParent, true);
                    UndoRedoManager::PushAction(
                        new UndoRedoCreateGameObject(modelGo));
                }
            }
        }
    }
}

bool Hierarchy::AcceptDragOrDrop(UIDragDroppable *dd)
{
    if (UITreeItemContainer *treeItemCont =
            DCAST<UITreeItemContainer *>(dd->GetGameObject()))
    {
        if (GameObject *containedItem = treeItemCont->GetContainedItem())
        {
            if (DCAST<HierarchyItem *>(containedItem))
            {
                return true;
            }
        }
    }

    if (ExplorerItem *expItem = DCAST<ExplorerItem *>(dd->GetGameObject()))
    {
        return Extensions::Equals(expItem->GetPath().GetExtension(),
                                  Extensions::GetModelExtensions());
    }

    return false;
}

UIEventResult Hierarchy::OnUIEvent(UIFocusable *, const UIEvent &event)
{
    switch (event.type)
    {
        case UIEvent::Type::MOUSE_CLICK_DOWN:
        {
            Editor::SelectGameObject(nullptr);
            return UIEventResult::INTERCEPT;
        }
        break;

        case UIEvent::Type::KEY_DOWN:
        {
            if (event.key.modifiers.IsOn(KeyModifier::LCTRL))
            {
                switch (event.key.key)
                {
                    case Key::V: OnPaste(nullptr); break;

                    default: break;
                }
            }
        }
        break;

        default: break;
    }
    return UIEventResult::IGNORE;
}

void Hierarchy::OnCreateContextMenu(MenuItem *menuRootItem)
{
    menuRootItem->SetFontSize(12);

    MenuItem *create = menuRootItem->AddItem("Create");
    MenuBar::CreateGameObjectCreateMenuInto(create);

    menuRootItem->AddSeparator();

    MenuItem *paste = menuRootItem->AddItem("Paste");
    paste->SetSelectedCallback([this](MenuItem *) { OnPaste(nullptr); });
    paste->SetOverAndActionEnabled(EditorClipboard::HasCopiedGameObject());
}

void Hierarchy::OnSceneLoaded(Scene *scene, const Path &)
{
    Clear();
    if (scene)
    {
        for (GameObject *go : scene->GetChildren())
        {
            AddGameObject(go);
        }
        scene->EventEmitter<IEventsChildren>::RegisterListener(this);
    }
}

void Hierarchy::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    if (GameObject *destroyedGo = DCAST<GameObject *>(object))
    {
        HierarchyItem *hItem = GetItemFromGameObject(destroyedGo);
        RemoveGameObjectItem(hItem);
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
    HierarchyItem *hItem = SCAST<HierarchyItem *>(item);
    hItem->OnSelectionCallback(action);
}

void Hierarchy::AddGameObject(GameObject *go)
{
    if (!go->HasComponent<HideInHierarchy>())
    {
        bool topItem = (go->GetScene() == go->GetParent());

        HierarchyItem *hItem = new HierarchyItem();
        hItem->SetReferencedGameObject(go);

        // Get index inside parent, without counting hidden ones
        int indexInsideParent = go->GetParent()->GetChildren().IndexOf(go);
        for (GameObject *child : go->GetParent()->GetChildren())
        {
            if (child->HasComponent<HideInHierarchy>())
            {
                --indexInsideParent;
            }
        }

        if (indexInsideParent >= 0)
        {
            HierarchyItem *parentItem = GetItemFromGameObject(go->GetParent());
            GetUITree()->AddItem(
                hItem, topItem ? nullptr : parentItem, indexInsideParent);
            m_gameObjectToItem.Add(go, hItem);

            for (GameObject *child : go->GetChildren())  // Add children too
            {
                AddGameObject(child);
            }
        }

        go->EventEmitter<IEventsChildren>::RegisterListener(this);
        go->EventEmitter<IEventsDestroy>::RegisterListener(this);
        hItem->EventEmitter<IEventsHierarchyItem>::RegisterListener(this);

        GetUITree()->SetItemCollapsed(hItem, true);
    }
}

void Hierarchy::RemoveGameObject(GameObject *go)
{
    UndoRedoManager::PushAction(new UndoRedoRemoveGameObject(go));

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

HierarchyItem *Hierarchy::GetItemFromGameObject(GameObject *go) const
{
    if (m_gameObjectToItem.ContainsKey(go))
    {
        return m_gameObjectToItem.Get(go);
    }
    return nullptr;
}

GameObject *Hierarchy::GetGameObjectFromItem(GOItem *item) const
{
    if (!item)
    {
        return nullptr;
    }
    ASSERT(DCAST<HierarchyItem *>(item));
    HierarchyItem *hItem = SCAST<HierarchyItem *>(item);
    return hItem->GetReferencedGameObject();
}

UITree *Hierarchy::GetUITree() const
{
    return p_uiTree;
}
