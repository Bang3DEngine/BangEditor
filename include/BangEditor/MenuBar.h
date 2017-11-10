#ifndef MENUBAR_H
#define MENUBAR_H

#include "BangEditor/BangEditor.h"

#include "Bang/Array.h"
#include "Bang/GameObject.h"
#include "BangEditor/MenuBarItem.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MenuBar : public GameObject
{
    GAMEOBJECT(MenuBar)

public:
    void Update() override;

    MenuBarItem* AddItem();
    MenuBarItem* GetItem(int i);

    static constexpr int GetFixedHeight() { return 18; }

protected:
    MenuBar();
    virtual ~MenuBar();

private:
    MenuBarItem *m_fileItem = nullptr;
    MenuBarItem *m_editItem = nullptr;
    MenuBarItem *m_assetsItem = nullptr;
    MenuBarItem *m_componentsItem  = nullptr;
    MenuBarItem *m_gameObjectsItem = nullptr;

    Array<MenuBarItem*> m_items;
    UIHorizontalLayout *m_horizontalLayout = nullptr;

    static void OnOpenProject (UIButton *btn);
    static void OnNewScene    (UIButton *btn);
    static void OnSaveScene   (UIButton *btn);
    static void OnOpenScene   (UIButton *btn);

    friend class Bang::ObjectManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // MENUBAR_H
