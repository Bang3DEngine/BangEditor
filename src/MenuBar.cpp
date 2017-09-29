#include "BangEditor/MenuBar.h"

#include "Bang/Dialog.h"
#include "Bang/Random.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/UIBorderRect.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"
#include "BangEditor/EditorScene.h"

USING_NAMESPACE_BANG_EDITOR

MenuBar::MenuBar()
{
    RectTransform *rt = AddComponent<RectTransform>();
    rt->SetAnchors(Vector2(-1.0f, 1.0f), Vector2(1.0f, 1.0f));
    rt->SetMarginBot(-GetFixedHeight());

    UIImageRenderer *bg = AddComponent<UIImageRenderer>();
    bg->SetTint(Color::LightGray);

    m_horizontalLayout = AddComponent<UIHorizontalLayout>();
    m_horizontalLayout->SetSpacing(5);

    m_fileItem        = AddItem();
    m_editItem        = AddItem();
    m_assetsItem      = AddItem();
    m_componentsItem  = AddItem();
    m_gameObjectsItem = AddItem();

    m_fileItem->GetText()->SetContent("File");
    m_editItem->GetText()->SetContent("Edit");
    m_assetsItem->GetText()->SetContent("Assets");
    m_componentsItem->GetText()->SetContent("Components");
    m_gameObjectsItem->GetText()->SetContent("GameObjects");

    MenuBarItem *newProject = m_fileItem->AddChild("New Project...");
    MenuBarItem *openProject = m_fileItem->AddChild("Open Project...");
    MenuBarItem *openRecentProject = m_fileItem->AddChild("Open Recent Project");
        openRecentProject->AddChild("Wololo/hehe.bproject");
        openRecentProject->AddChild("Wololo/hoho.bproject");
        openRecentProject->AddChild("Wololo/bangbangbang.bproject");

    MenuBarItem *saveProject = m_fileItem->AddChild("Save Project");
    MenuBarItem *saveProjectAs = m_fileItem->AddChild("Save Project As...");
    m_fileItem->AddSeparator();
    MenuBarItem *newScene = m_fileItem->AddChild("New Scene");
    MenuBarItem *saveScene = m_fileItem->AddChild("Save Scene");
    MenuBarItem *saveSceneAs= m_fileItem->AddChild("Save Scene As...");

    MenuBarItem *copyChild = m_editItem->AddChild("Copy");

    openProject->GetButton()->AddClickedCallback(
        [](UIButton*)
        {
            Dialog::GetFilePath("Open Project...");
        }
    );

    newScene->GetButton()->AddClickedCallback(
        [](UIButton*)
        {
            EditorScene *edScene = SCAST<EditorScene*>(SceneManager::GetRootScene());
            Scene *defaultScene = GameObjectFactory::CreateDefaultScene();
            edScene->SetOpenScene(defaultScene);
        }
    );
}

MenuBar::~MenuBar()
{
}

void MenuBar::Update()
{
    UIGameObject::Update();
    EditorScene *edScene = SCAST<EditorScene*>(SceneManager::GetRootScene());
    if (Input::GetKeyDown(Key::E))
    {
        edScene->SetOpenScene( GameObjectFactory::CreateDefaultScene() );
    }

    Scene *openScene = edScene->GetOpenScene();
    if (openScene)
    {
        if (Input::GetKeyDownRepeat(Key::C))
        {
            GameObject *sphere = MeshFactory::GetSphereGameObject();
            sphere->transform->SetPosition( Random::InsideUnitSphere() );
            sphere->transform->SetScale( Random::InsideUnitSphere() + Vector3(0.3f) );
            openScene->AddChild(sphere);
        }
    }
}

MenuBarItem* MenuBar::AddItem()
{
    MenuBarItem *item = new MenuBarItem(false);
    item->SetParent(this);
    m_items.PushBack(item);
    return item;
}

MenuBarItem* MenuBar::GetItem(int i)
{
    return m_items[i];
}
