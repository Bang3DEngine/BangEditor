#ifndef EXPLORER_H
#define EXPLORER_H

#include "Bang/Path.h"
#include "Bang/UIButton.h"

#include "BangEditor/EditorUITab.h"
#include "BangEditor/ProjectManager.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UILabel;
FORWARD class UIScrollPanel;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ExplorerItem;

class Explorer : public EditorUITab,
                 public ProjectManagerListener
{
    GAMEOBJECT_EDITOR(Explorer);

public:
    Explorer();
    virtual ~Explorer();

    void SetRootPath(const Path &rootPath);
    void SetCurrentPath(const Path &path);

    const Path &GetRootPath() const;
    const Path &GetCurrentPath() const;

    void Clear();

    // ProjectManagerListener
    void OnProjectOpen(const Project *project) override;
    void OnProjectClosed(const Project *project) override;

    static Explorer *GetInstance();

private:
    Path m_rootPath = Path::Empty;
    Path m_currentPath = Path::Empty;
    List<ExplorerItem*> p_items;

    UILabel *p_directionLabel = nullptr;
    UIButton *p_backButton = nullptr;
    UIScrollPanel *p_scrollPanel = nullptr;
    GameObject *p_itemsContainer = nullptr;

    void AddItem(const Path &itemPath);
    void GoDirectoryUp();

    bool IsInsideRootPath(const Path &path) const;
};

NAMESPACE_BANG_EDITOR_END


#endif // EXPLORER_H
