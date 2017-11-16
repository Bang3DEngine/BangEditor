#ifndef MENUBAR_H
#define MENUBAR_H

#include "BangEditor/BangEditor.h"

#include "Bang/Array.h"
#include "Bang/GameObject.h"
#include "BangEditor/MenuItem.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MenuBar : public GameObject
{
    GAMEOBJECT(MenuBar)

public:
    void Update() override;

    MenuItem* AddItem();
    MenuItem* GetItem(int i);

    static constexpr int GetFixedHeight() { return 18; }

protected:
    MenuBar();
    virtual ~MenuBar();

private:
    MenuItem *m_fileItem = nullptr;
    MenuItem *m_editItem = nullptr;
    MenuItem *m_assetsItem = nullptr;
    MenuItem *m_componentsItem  = nullptr;
    MenuItem *m_gameObjectsItem = nullptr;

    Array<MenuItem*> m_items;
    UIHorizontalLayout *m_horizontalLayout = nullptr;

    static void OnOpenProject (UIButton *btn);
    static void OnNewScene    (UIButton *btn);
    static void OnSaveScene   (UIButton *btn);
    static void OnOpenScene   (UIButton *btn);

    friend class Bang::ObjectManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // MENUBAR_H
