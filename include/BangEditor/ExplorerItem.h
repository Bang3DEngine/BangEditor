#ifndef EXPLORERITEM_H
#define EXPLORERITEM_H

#include "Bang/Path.h"
#include "Bang/GameObject.h"
#include "Bang/IFocusListener.h"

#include "BangEditor/UIContextMenu.h"

USING_NAMESPACE_BANG

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UILabel;
FORWARD class IFocusable;
FORWARD class UIFocusable;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

NAMESPACE_BANG_EDITOR_BEGIN

class ExplorerItem : public GameObject,
                     public IFocusListener
{
    GAMEOBJECT_EDITOR(ExplorerItem);

public:
    void Update() override;

    void SetPath(const Path &path);
    void SetSelected(bool selected);

    bool IsSelected() const;
    UILabel *GetLabel() const;
    const Path& GetPath() const;
    UIFocusable* GetFocusable() const;

    void RenamePath();
    void RemovePath();

    // UIContextMenu callback
    void OnCreateContextMenu(MenuItem *menuRootItem);

protected:
    ExplorerItem();
    virtual ~ExplorerItem();

private:
    bool m_selected = false;
    Path m_path = Path::Empty;

    UILabel *p_label = nullptr;
    UIFocusable *p_button = nullptr;
    UIImageRenderer *p_bg = nullptr;
    UIImageRenderer *p_icon = nullptr;
    UIContextMenu *p_contextMenu = nullptr;

    // IFocusListener
    virtual void OnMouseEnter(IFocusable*) override;
    virtual void OnMouseExit(IFocusable*) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // EXPLORERITEM_H

