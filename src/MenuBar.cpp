#include "BangEditor/MenuBar.h"

#include "Bang/Scene.h"
#include "Bang/Dialog.h"
#include "Bang/Random.h"
#include "Bang/Extensions.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/UIBorderRect.h"
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
            sphere->GetTransform()->SetPosition( Random::InsideUnitSphere() );
            sphere->GetTransform()->SetScale( Vector3(Random::Range(0.3f, 1.0f)) );
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
        Path saveScenePath = Dialog::GetFilePath("Save Scene...",
                                                 { Extensions::Get<Scene>() });
        if (saveScenePath.IsFile())
        {
            openScene->ExportXMLToFile( Path(saveScenePath) );
        }
    }
}

void MenuBar::OnOpenScene(UIButton*)
{
    Path openScenePath = Dialog::GetFilePath("Open Scene...",
                                             { Extensions::Get<Scene>() });
    if (openScenePath.IsFile())
    {
        EditorScene *edScene = EditorScene::GetInstance();
        Scene *scene = new Scene();
        scene->ImportXMLFromFile(openScenePath);
        edScene->SetOpenScene(scene);
    }
}
