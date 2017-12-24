#include "BangEditor/MenuBar.h"

#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Dialog.h"
#include "Bang/Random.h"
#include "Bang/Extensions.h"
#include "Bang/IFocusable.h"
#include "Bang/MeshFactory.h"
#include "Bang/UIFocusable.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/Project.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSettings.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG_EDITOR

MenuBar::MenuBar()
{
    SetName("MenuBar");
    GameObjectFactory::CreateUIGameObjectInto(this);

    // UIImageRenderer *bg = AddComponent<UIImageRenderer>();
    // bg->SetTint(Color::Gray);

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

    MenuItem *newProject = m_fileItem->AddItem("New Project...");
    MenuItem *openProject = m_fileItem->AddItem("Open Project...");
    MenuItem *openRecentProject = m_fileItem->AddItem("Open Recent Project");
        openRecentProject->AddItem("Wololo/hehe.bproject");
        openRecentProject->AddItem("Wololo/hoho.bproject");
        openRecentProject->AddItem("Wololo/bangbangbang.bproject");

    m_fileItem->AddSeparator();
    MenuItem *newScene = m_fileItem->AddItem("New Scene");
    MenuItem *openScene = m_fileItem->AddItem("Open Scene");
    MenuItem *saveScene = m_fileItem->AddItem("Save Scene");
    MenuItem *saveSceneAs = m_fileItem->AddItem("Save Scene As...");
    MenuItem *closeScene = m_fileItem->AddItem("Close Scene");

    MenuItem *copyChild = m_editItem->AddItem("Copy");

    newProject->GetButton()->AddClickedCallback(MenuBar::OnNewProject);
    openProject->GetButton()->AddClickedCallback(MenuBar::OnOpenProject);
    newScene->GetButton()->AddClickedCallback(MenuBar::OnNewScene);
    saveScene->GetButton()->AddClickedCallback(MenuBar::OnSaveScene);
    openScene->GetButton()->AddClickedCallback(MenuBar::OnOpenScene);
}

MenuBar::~MenuBar()
{
}

void MenuBar::Update()
{
    GameObject::Update();
    EditorScene *edScene = EditorSceneManager::GetEditorScene();
    if (Input::GetKeyDown(Key::Q))
    {
        MenuBar::OnNewScene(nullptr);
    }
    else if (Input::GetKeyDown(Key::O))
    {
        MenuBar::OnOpenScene(nullptr);
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

MenuItem* MenuBar::AddItem()
{
    MenuItem *item = GameObject::Create<MenuItem>( MenuItem::MenuItemType::Top );
    item->SetParent(this);
    m_items.PushBack(item);
    return item;
}

MenuItem* MenuBar::GetItem(int i)
{
    return m_items[i];
}

void MenuBar::OnNewProject(IFocusable*)
{
    Path newProjectDirPath = Dialog::OpenDirectory("Create New Project...");
    if (newProjectDirPath.IsDir())
    {
        String projectName = Dialog::GetString("Choose Project Name",
                                               "Please, choose your project name:",
                                               "NewProject");
        if (!projectName.IsEmpty())
        {
            Project *proj = ProjectManager::CreateNewProject(newProjectDirPath,
                                                             projectName);
            MenuBar::OpenProject(proj->GetProjectFileFilepath());
        }
    }
}

void MenuBar::OnOpenProject(IFocusable*)
{
    Debug_Peek(Extensions::GetProjectExtension());
    Path projectFileFilepath = Dialog::OpenFilePath("Open Project...",
                                                    {Extensions::GetProjectExtension()});
    OpenProject(projectFileFilepath);
}
void MenuBar::OpenProject(const Path &projectFileFilepath)
{
    if (projectFileFilepath.IsFile())
    {
        ProjectManager::OpenProject(projectFileFilepath);
    }
}

void MenuBar::OnNewScene(IFocusable*)
{
    MenuBar::CloseScene();

    EditorScene *edScene = EditorSceneManager::GetEditorScene();

    Scene *previousOpenScene = EditorSceneManager::GetOpenScene();
    if (previousOpenScene) { GameObject::Destroy(previousOpenScene); }

    Scene *defaultScene = GameObjectFactory::CreateScene();
    edScene->SetOpenScene(defaultScene);

    GameObjectFactory::CreateDefaultSceneInto(defaultScene);
    defaultScene->SetFirstFoundCamera();
}

void MenuBar::OnSaveScene(IFocusable*)
{
    EditorScene *edScene = EditorSceneManager::GetEditorScene();
    Scene *openScene = edScene->GetOpenScene();
    if (openScene)
    {
        Path saveScenePath = Dialog::SaveFilePath("Save Scene...",
                                       { Extensions::GetSceneExtension() });
        // if (saveScenePath.IsFile())
        {
            openScene->ExportXMLToFile( Path(saveScenePath) );
        }
    }
}

void MenuBar::OnOpenScene(IFocusable*)
{
    MenuBar::CloseScene();

    Path openScenePath = Dialog::OpenFilePath("Open Scene...",
                                             { Extensions::GetSceneExtension() });
    if (openScenePath.IsFile())
    {
        EditorScene *edScene = EditorSceneManager::GetEditorScene();
        Scene *scene = GameObjectFactory::CreateScene(false);
        scene->ImportXMLFromFile(openScenePath);
        edScene->SetOpenScene(scene);
    }
}

void MenuBar::OnCloseScene(IFocusable*)
{
    MenuBar::CloseScene();
}
void MenuBar::CloseScene()
{
    EditorSceneManager::GetEditorScene()->SetOpenScene(nullptr);
}
