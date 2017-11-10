#ifndef EXPLORER_H
#define EXPLORER_H

#include "Bang/Path.h"
#include "Bang/UIButton.h"
#include "Bang/UIButtonDriver.h"

#include "BangEditor/EditorUITab.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UILabel;
FORWARD class UIScrollPanel;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ExplorerEntry;

class Explorer : public EditorUITab,
                 public IUIButtonListener
{
    GAMEOBJECT_EDITOR(Explorer)

public:
    Explorer();
    virtual ~Explorer();

    void SetCurrentPath(const Path &path);
    const Path &GetCurrentPath() const;

    void Clear();

    static Explorer *GetInstance();

private:
    Path m_currentPath = Path::Empty;
    List<ExplorerEntry*> p_entries;

    UILabel *p_directionLabel = nullptr;
    UIButtonDriver *p_backButton = nullptr;
    UIScrollPanel *p_scrollPanel = nullptr;
    GameObject *p_entriesContainer = nullptr;

    void AddEntry(const Path &entryPath);

    virtual void OnButton_Clicked(UIButton *btn) override;
};

NAMESPACE_BANG_EDITOR_END


#endif // EXPLORER_H
