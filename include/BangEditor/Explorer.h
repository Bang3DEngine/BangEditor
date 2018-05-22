#ifndef EXPLORER_H
#define EXPLORER_H

#include "Bang/Path.h"
#include "Bang/UIButton.h"
#include "Bang/FileTracker.h"
#include "Bang/IValueChangedListener.h"

#include "BangEditor/Editor.h"
#include "BangEditor/ExplorerItem.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/ShortcutManager.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UILabel;
FORWARD class UISlider;
FORWARD class UIGridLayout;
FORWARD class UIScrollPanel;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class Explorer : public GameObject,
                 public IEditorListener,
                 public IFileTrackerListener,
                 public IValueChangedListener,
                 public IExplorerItemListener,
                 public IProjectManagerListener
{
    GAMEOBJECT_EDITOR(Explorer);

public:
    Explorer();
    virtual ~Explorer();

    void Update() override;

    void ForceCheckFileChanges();
    void SelectPath(const Path &path);
    void SetRootPath(const Path &rootPath);
    void SetCurrentPath(const Path &path);

    const Path &GetRootPath() const;
    const Path &GetCurrentPath() const;
    const Path &GetSelectedPath() const;

    void Clear();

    // IProjectManagerListener
    void OnProjectOpen(const Project *project) override;
    void OnProjectClosed(const Project *project) override;

    // IFileTrackerListener
    void OnPathAdded(const Path &addedPath) override;
    void OnPathModified(const Path &modifiedPath) override;
    void OnPathRemoved(const Path &removedPath) override;

    // IEditorListener
    void OnGameObjectSelected(GameObject *selectedGameObject) override;

    // IExplorerItemListener
    virtual void OnRename(ExplorerItem *item) override;
    virtual void OnRemove(ExplorerItem *item) override;
    virtual void OnDuplicate(ExplorerItem *item) override;
    virtual void OnDropped(ExplorerItem *item) override;

    static Explorer *GetInstance();

private:
    Path m_rootPath = Path::Empty;
    Path m_currentPath = Path::Empty;
    List<ExplorerItem*> p_items;
    Map<Path, ExplorerItem*> m_pathsToItem;

    UIButton *p_backButton = nullptr;
    UISlider *p_iconSizeSlider = nullptr;
    UILabel *p_currentPathLabel = nullptr;
    UIScrollPanel *p_scrollPanel = nullptr;
    GameObject *p_itemsContainer = nullptr;
    UIGridLayout *p_explorerGridLayout = nullptr;

    void AddItem(const Path &itemPath);
    void AddItem(ExplorerItem *explorerItem);
    void RemoveItem(const Path &itemPath);
    void GoDirectoryUp();

    void RenamePath(ExplorerItem *explorerItem);
    void RemovePath(ExplorerItem *explorerItem);
    void DuplicatePath(ExplorerItem *explorerItem);

    ExplorerItem *GetSelectedItem() const;
    ExplorerItem *GetItemFromPath(const Path &path) const;

    static void OnItemDoubleClicked(IFocusable*);

    static void OnShortcutPressed(const Shortcut &shortcut);

    bool IsInsideRootPath(const Path &path) const;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;
};

NAMESPACE_BANG_EDITOR_END


#endif // EXPLORER_H
