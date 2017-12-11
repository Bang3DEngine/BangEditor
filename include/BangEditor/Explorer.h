#ifndef EXPLORER_H
#define EXPLORER_H

#include "Bang/Path.h"
#include "Bang/UIButton.h"

#include "BangEditor/EditorUITab.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UILabel;
FORWARD class UIScrollPanel;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ExplorerItem;

class Explorer : public EditorUITab
{
    GAMEOBJECT_EDITOR(Explorer);

public:
    Explorer();
    virtual ~Explorer();

    void SetCurrentPath(const Path &path);
    const Path &GetCurrentPath() const;

    void Clear();

    static Explorer *GetInstance();

private:
    Path m_currentPath = Path::Empty;
    List<ExplorerItem*> p_items;

    UILabel *p_directionLabel = nullptr;
    UIButton *p_backButton = nullptr;
    UIScrollPanel *p_scrollPanel = nullptr;
    GameObject *p_itemsContainer = nullptr;

    void AddItem(const Path &itemPath);
    void GoDirectoryUp();
};

NAMESPACE_BANG_EDITOR_END


#endif // EXPLORER_H
