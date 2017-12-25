#ifndef MENUBAR_H
#define MENUBAR_H

#include "BangEditor/BangEditor.h"

#include "Bang/Array.h"
#include "Bang/GameObject.h"
#include "BangEditor/MenuItem.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/SceneOpenerSaver.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MenuBar : public GameObject
{
    GAMEOBJECT(MenuBar);

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

    SceneOpenerSaver *m_sceneOpenerSaver = nullptr;

    static void OnNewProject  (IFocusable *btn);
    static void OnOpenProject (IFocusable *btn);
    static void OnNewScene    (IFocusable *btn);
    static void OnSaveScene   (IFocusable *btn);
    static void OnSaveSceneAs (IFocusable *btn);
    static void OnOpenScene   (IFocusable *btn);

    static void OpenProject(const Path& projectFileFilepath);

    static MenuBar* GetInstance();

    friend class ProjectManager;
    friend class Bang::ObjectManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // MENUBAR_H
