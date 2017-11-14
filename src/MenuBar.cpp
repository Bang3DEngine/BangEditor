#include "BangEditor/MenuBar.h"

#include "Bang/Scene.h"
#include "Bang/Dialog.h"
#include "Bang/Random.h"
#include "Bang/Extensions.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/EditorScene.h"

USING_NAMESPACE_BANG_EDITOR

MenuBar::MenuBar()
{
    SetName("MenuBar");
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIImageRenderer *bg = AddComponent<UIImageRenderer>();
    bg->SetTint(Color::LightGray);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinHeight(15);

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

    MenuBarItem *newProject = m_fileItem->SetAsChild("New Project...");
    MenuBarItem *openProject = m_fileItem->SetAsChild("Open Project...");
    MenuBarItem *openRecentProject = m_fileItem->SetAsChild("Open Recent Project");
        openRecentProject->SetAsChild("Wololo/hehe.bproject");
        openRecentProject->SetAsChild("Wololo/hoho.bproject");
        openRecentProject->SetAsChild("Wololo/bangbangbang.bproject");

    MenuBarItem *saveProject = m_fileItem->SetAsChild("Save Project");
    MenuBarItem *saveProjectAs = m_fileItem->SetAsChild("Save Project As...");
    m_fileItem->AddSeparator();
    MenuBarItem *newScene = m_fileItem->SetAsChild("New Scene");
    MenuBarItem *openScene = m_fileItem->SetAsChild("Open Scene");
    MenuBarItem *saveScene = m_fileItem->SetAsChild("Save Scene");
    MenuBarItem *saveSceneAs= m_fileItem->SetAsChild("Save Scene As...");

    MenuBarItem *copyChild = m_editItem->SetAsChild("Copy");

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
    EditorScene *edScene = EditorScene::GetInstance();
    if (Input::GetKeyDown(Key::E))
    {
        MenuBar::OnNewScene(nullptr);
    }
    else if (Input::GetKeyDown(Key::O))
    {
        MenuBar::OnOpenProject(nullptr);
    }

    Scene *openScene = edScene->GetOpenScene();
    if (openScene)
    {
        if (Input::GetKeyDownRepeat(Key::C))
        {
            GameObject *sphere = MeshFactory::GetSphereGameObject();
            sphere->GetTransform()->SetPosition( Random::GetInsideUnitSphere() );
            sphere->GetTransform()->SetScale( Vector3(Random::GetRange(0.3f, 1.0f)) );
            openScene->SetAsChild(sphere);
        }
    }
}

MenuBarItem* MenuBar::AddItem()
{
    MenuBarItem *item = ObjectManager::Create<MenuBarItem>(true);
    item->SetParent(this);
    m_items.PushBack(item);
    return item;
}

MenuBarItem* MenuBar::GetItem(int i)
{
    return m_items[i];
}

void MenuBar::OnOpenProject(UIButton*)
{
    Dialog::GetFilePath("Open Project...");
}

void MenuBar::OnNewScene(UIButton*)
{
    EditorScene *edScene = EditorScene::GetInstance();
    Scene *defaultScene = GameObjectFactory::CreateDefaultScene();
    edScene->SetOpenScene(defaultScene);
}

void MenuBar::OnSaveScene(UIButton*)
{
    EditorScene *edScene = EditorScene::GetInstance();
    Scene *openScene = edScene->GetOpenScene();
    if (openScene)
    {
        Path saveScenePath =
                Dialog::GetFilePath("Save Scene...",
                                    { Extensions::GetSceneExtension() });
        if (saveScenePath.IsFile())
        {
            openScene->ExportXMLToFile( Path(saveScenePath) );
        }
    }
}

void MenuBar::OnOpenScene(UIButton*)
{
    Path openScenePath = Dialog::GetFilePath("Open Scene...",
                                             { Extensions::GetSceneExtension() });
    if (openScenePath.IsFile())
    {
        EditorScene *edScene = EditorScene::GetInstance();
        Scene *scene = GameObjectFactory::CreateScene();
        scene->ImportXMLFromFile(openScenePath);
        edScene->SetOpenScene(scene);
    }
}
