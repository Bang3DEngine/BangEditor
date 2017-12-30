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

    static void OnNewProject  (MenuItem *item);
    static void OnOpenProject (MenuItem *item);
    static void OnNewScene    (MenuItem *item);
    static void OnSaveScene   (MenuItem *item);
    static void OnSaveSceneAs (MenuItem *item);
    static void OnOpenScene   (MenuItem *item);

    static void OpenProject(const Path& projectFileFilepath);

    static MenuBar* GetInstance();

    friend class ProjectManager;
    friend class SceneOpenerSaver;
    friend class Bang::ObjectManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // MENUBAR_H
