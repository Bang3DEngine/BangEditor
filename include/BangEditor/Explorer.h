#ifndef EXPLORER_H
#define EXPLORER_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/FileTracker.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsFileTracker.h"
#include "Bang/IEventsFocus.h"
#include "Bang/IEventsProjectManager.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/List.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/UIButton.h"
#include "Bang/UMap.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/Editor.h"
#include "BangEditor/EditorProjectManager.h"
#include "BangEditor/ExplorerItem.h"
#include "BangEditor/ShortcutManager.h"

namespace Bang
{
class UIButton;
class UIFocusable;
class UIGridLayout;
class UILabel;
class UIScrollPanel;
class UISlider;
template <class>
class EventEmitter;
class Project;
}  // namespace Bang

using namespace Bang;
namespace BangEditor
{
class ExplorerItem;
class IEventsEditor;
class MenuItem;
class UIContextMenu;

class Explorer : public GameObject,
                 public EventListener<IEventsFocus>,
                 public EventListener<IEventsEditor>,
                 public EventListener<IEventsFileTracker>,
                 public EventListener<IEventsValueChanged>,
                 public EventListener<IEventsExplorerItem>,
                 public EventListener<IEventsProjectManager>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(Explorer);

public:
    Explorer();
    virtual ~Explorer() override;

    void Update() override;

    void ForceCheckFileChanges();
    void SelectPath(const Path &path,
                    bool registerUndo = true,
                    bool travelToDirectory = true);
    void SetRootPath(const Path &rootPath);
    void SetCurrentPath(const Path &path);

    const Path &GetRootPath() const;
    const Path &GetCurrentPath() const;
    const Path &GetSelectedPath() const;

    void Clear();

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

    // IEventsProjectManager
    void OnProjectOpen(const Project *project) override;
    void OnProjectClosed(const Project *project) override;

    // IFileTrackerListener
    void OnPathAdded(const Path &addedPath) override;
    void OnPathModified(const Path &modifiedPath) override;
    void OnPathRemoved(const Path &removedPath) override;

    // IEditorListener
    void OnGameObjectSelected(GameObject *selectedGameObject) override;

    // IEventsExplorerItem
    virtual void OnRename(ExplorerItem *item) override;
    virtual void OnRemove(ExplorerItem *item) override;
    virtual void OnDuplicate(ExplorerItem *item) override;
    virtual void OnPastedOver(ExplorerItem *item) override;
    virtual void OnDroppedToDirectory(ExplorerItem *item) override;

    static Explorer *GetInstance();

private:
    Path m_rootPath = Path::Empty();
    Path m_currentPath = Path::Empty();
    Path m_selectedPath = Path::Empty();

    List<ExplorerItem *> p_items;
    UMap<Path, ExplorerItem *> m_pathsToItem;

    UIButton *p_backButton = nullptr;
    UISlider *p_iconSizeSlider = nullptr;
    UILabel *p_currentPathLabel = nullptr;
    UIScrollPanel *p_scrollPanel = nullptr;
    GameObject *p_itemsContainer = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    UIGridLayout *p_explorerGridLayout = nullptr;

    void AddItem(const Path &itemPath);
    void AddItem(ExplorerItem *explorerItem);
    void RemoveItem(const Path &itemPath);
    void GoDirectoryUp();

    void DuplicatePathIntoDir(const Path &srcPath, const Path &dstDirPath);

    ExplorerItem *GetSelectedItem() const;
    ExplorerItem *GetItemFromPath(const Path &path) const;

    void OnItemDoubleClicked(UIFocusable *);

    bool IsInsideRootPath(const Path &path) const;

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;

    // UIContextMenu
    virtual void OnCreateContextMenu(MenuItem *menuRootItem);

    friend class UndoRedoExplorerSelect;
};
}  // namespace BangEditor

#endif  // EXPLORER_H
