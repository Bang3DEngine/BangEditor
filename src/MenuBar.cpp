#include "BangEditor/MenuBar.h"

#include "Bang/Scene.h"
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
    MenuBarItem *openScene = m_fileItem->AddChild("Open Scene");
    MenuBarItem *saveScene = m_fileItem->AddChild("Save Scene");
    MenuBarItem *saveSceneAs= m_fileItem->AddChild("Save Scene As...");

    MenuBarItem *copyChild = m_editItem->AddChild("Copy");

    openProject->GetButton()->AddClickedCallback(MenuBar::OnOpenProject);
    newScene->GetButton()->AddClickedCallback(MenuBar::OnNewScene);
    saveScene->GetButton()->AddClickedCallback(MenuBar::OnSaveScene);
    openScene->GetButton()->AddClickedCallback(MenuBar::OnOpenScene);
}

MenuBar::~MenuBar()
{
}

#include "Bang/UILayoutManager.h"
void MenuBar::Update()
{
    GameObject::Update();
    EditorScene *edScene = SCAST<EditorScene*>(SceneManager::GetRootScene());
    if (Input::GetKeyDown(Key::A))
    {
        UILayoutManager::InvalidateAll(edScene);
        UILayoutManager::ForceRebuildLayout(edScene);
    }
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
            sphere->transform->SetScale( Vector3(Random::Range(0.3f, 1.0f)) );
            openScene->AddChild(sphere);
        }
    }
}

MenuBarItem* MenuBar::AddItem()
{
    MenuBarItem *item = new MenuBarItem(true);
    item->SetParent(this);
    m_items.PushBack(item);
    return item;
}

MenuBarItem* MenuBar::GetItem(int i)
{
    return m_items[i];
}

void MenuBar::OnOpenProject(UIButton *btn)
{
    Dialog::GetFilePath("Open Project...");
}

void MenuBar::OnNewScene(UIButton *btn)
{
    EditorScene *edScene = SCAST<EditorScene*>(SceneManager::GetRootScene());
    Scene *defaultScene = GameObjectFactory::CreateDefaultScene();
    edScene->SetOpenScene(defaultScene);
}

void MenuBar::OnSaveScene(UIButton *btn)
{
    Path saveScenePath = Dialog::GetFilePath("Save Scene...");
    EditorScene *edScene = SCAST<EditorScene*>(SceneManager::GetRootScene());
    Scene *openScene = edScene->GetOpenScene();
    if (openScene)
    {
        openScene->ExportXMLToFile( Path("Test.bscene") );
    }
}

void MenuBar::OnOpenScene(UIButton *btn)
{
    Path openScenePath = Dialog::GetFilePath("Open Scene...");
    EditorScene *edScene = SCAST<EditorScene*>(SceneManager::GetRootScene());
    Scene *scene = new Scene();
    scene->ImportXMLFromFile( Path("Test.bscene") );
    edScene->SetOpenScene(scene);
}
