#include "BangEditor/MenuBar.h"

#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Thread.h"
#include "Bang/Camera.h"
#include "Bang/Dialog.h"
#include "Bang/Random.h"
#include "Bang/UICanvas.h"
#include "Bang/Transform.h"
#include "Bang/Extensions.h"
#include "Bang/IFocusable.h"
#include "Bang/PointLight.h"
#include "Bang/SystemUtils.h"
#include "Bang/AudioSource.h"
#include "Bang/UIFocusable.h"
#include "Bang/SceneManager.h"
#include "Bang/LineRenderer.h"
#include "Bang/MeshRenderer.h"
#include "Bang/AudioListener.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/DirectionalLight.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/BehaviourContainer.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/Editor.h"
#include "BangEditor/Project.h"
#include "BangEditor/MenuItem.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/GameBuilder.h"
#include "BangEditor/EditorDialog.h"
#include "BangEditor/EditorSettings.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/SceneOpenerSaver.h"
#include "BangEditor/BehaviourCreator.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG_EDITOR

MenuBar::MenuBar()
{
    SetName("MenuBar");
    GameObjectFactory::CreateUIGameObjectInto(this);

    m_sceneOpenerSaver = new SceneOpenerSaver();

    p_focusable = AddComponent<UIFocusable>();

    UIImageRenderer *bg = AddComponent<UIImageRenderer>();
    bg->SetTint( Color::LightGray.WithValue(0.9f) );

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinHeight(15);

    m_horizontalLayout = AddComponent<UIHorizontalLayout>();
    m_horizontalLayout->SetSpacing(5);

    m_fileItem        = AddItem();
    m_editItem        = AddItem();
    m_assetsItem      = AddItem();
    m_componentsItem  = AddItem();
    m_gameObjectsItem = AddItem();

    // Top items
    m_fileItem->GetText()->SetContent("File");
    m_editItem->GetText()->SetContent("Edit");
    m_assetsItem->GetText()->SetContent("Assets");
    m_componentsItem->GetText()->SetContent("Components");
    m_gameObjectsItem->GetText()->SetContent("GameObjects");

    // File
    MenuItem *newProject = m_fileItem->AddItem("New Project...");
    MenuItem *openProject = m_fileItem->AddItem("Open Project...");

    m_fileItem->AddSeparator();
    MenuItem *newScene = m_fileItem->AddItem("New Scene");
    MenuItem *openScene = m_fileItem->AddItem("Open Scene");
    MenuItem *saveScene = m_fileItem->AddItem("Save Scene");
    MenuItem *saveSceneAs = m_fileItem->AddItem("Save Scene As...");

    m_fileItem->AddSeparator();
    MenuItem *build = m_fileItem->AddItem("Build");
    MenuItem *buildAndRun = m_fileItem->AddItem("Build and run");

    newProject->SetSelectedCallback(MenuBar::OnNewProject);
    openProject->SetSelectedCallback(MenuBar::OnOpenProject);
    newScene->SetSelectedCallback(MenuBar::OnNewScene);
    saveScene->SetSelectedCallback(MenuBar::OnSaveScene);
    saveSceneAs->SetSelectedCallback(MenuBar::OnSaveSceneAs);
    openScene->SetSelectedCallback(MenuBar::OnOpenScene);
    build->SetSelectedCallback(MenuBar::OnBuild);
    buildAndRun->SetSelectedCallback(MenuBar::OnBuildAndRun);

    // Components
    MenuItem *addAudio = m_componentsItem->AddItem("Audio");
    MenuItem *addAudioListener = addAudio->AddItem("Audio Listener");
    MenuItem *addAudioSource = addAudio->AddItem("Audio Source");
    MenuItem *addBehaviours = m_componentsItem->AddItem("Behaviour");
    MenuItem *addNewBehaviour = addBehaviours->AddItem("New Behaviour...");
    MenuItem *addExistingBehaviour = addBehaviours->AddItem("Existing Behaviour...");
    MenuItem *addEmptyBehaviour = addBehaviours->AddItem("Empty Behaviour");
    MenuItem *addCamera = m_componentsItem->AddItem("Camera");
    MenuItem *addLight = m_componentsItem->AddItem("Light");
    MenuItem *addPointLight = addLight->AddItem("Point Light");
    MenuItem *addDirectionalLight = addLight->AddItem("Directional Light");
    MenuItem *addRenderer = m_componentsItem->AddItem("Renderer");
    MenuItem *addLineRenderer = addRenderer->AddItem("LineRenderer");
    MenuItem *addMeshRenderer = addRenderer->AddItem("MeshRenderer");
    MenuItem *addTransforms = m_componentsItem->AddItem("Transform");
    MenuItem *addTransform = addTransforms->AddItem("Transform");
    MenuItem *addRectTransform = addTransforms->AddItem("RectTransform");

    addAudioListener->SetSelectedCallback(MenuBar::OnAddAudioListener);
    addAudioSource->SetSelectedCallback(MenuBar::OnAddAudioSource);
    addNewBehaviour->SetSelectedCallback(MenuBar::OnAddNewBehaviour);
    addExistingBehaviour->SetSelectedCallback(MenuBar::OnAddExistingBehaviour);
    addEmptyBehaviour->SetSelectedCallback(MenuBar::OnAddEmptyBehaviour);
    addCamera->SetSelectedCallback(MenuBar::OnAddCamera);
    addPointLight->SetSelectedCallback(MenuBar::OnAddPointLight);
    addDirectionalLight->SetSelectedCallback(MenuBar::OnAddDirectionalLight);
    addLineRenderer->SetSelectedCallback(MenuBar::OnAddLineRenderer);
    addMeshRenderer->SetSelectedCallback(MenuBar::OnAddMeshRenderer);
    addTransform->SetSelectedCallback(MenuBar::OnAddTransform);
    addRectTransform->SetSelectedCallback(MenuBar::OnAddRectTransform);

    // GameObject
    MenuItem *primitiveGameObjectItem = m_gameObjectsItem->AddItem("Primitives");
    MenuItem *createCone   = primitiveGameObjectItem->AddItem("Cone");
    MenuItem *createCube   = primitiveGameObjectItem->AddItem("Cube");
    MenuItem *createSphere = primitiveGameObjectItem->AddItem("Sphere");
    MenuItem *createPlane  = primitiveGameObjectItem->AddItem("Plane");

    MenuItem *uiGameObjectItem = m_gameObjectsItem->AddItem("UI");
    MenuItem *createMeh   = uiGameObjectItem->AddItem("Meh");

    createCone->SetSelectedCallback(MenuBar::OnCreateCone);
    createCube->SetSelectedCallback(MenuBar::OnCreateCube);
    createPlane->SetSelectedCallback(MenuBar::OnCreatePlane);
    createSphere->SetSelectedCallback(MenuBar::OnCreateSphere);
}

MenuBar::~MenuBar()
{
    delete m_sceneOpenerSaver;
}

void MenuBar::Update()
{
    GameObject::Update();

    UICanvas *canvas = UICanvas::GetActive(this);
    bool hasFocusRecursive = canvas->HasFocus(this, true);
    for (MenuItem *item : m_items)
    {
        item->SetMenuEnabled(hasFocusRecursive);
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

void MenuBar::OnNewProject(MenuItem*)
{
    Path newProjectDirPath = Dialog::OpenDirectory("Create New Project...",
                                                   EditorPaths::GetHome());
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
void MenuBar::OnOpenProject(MenuItem*)
{
    Path projectFileFilepath = Dialog::OpenFilePath("Open Project...",
                                                    {Extensions::GetProjectExtension()},
                                                     EditorPaths::GetHome());
    OpenProject(projectFileFilepath);
}
void MenuBar::OpenProject(const Path &projectFileFilepath)
{
    if (projectFileFilepath.IsFile())
    {
        ProjectManager::OpenProject(projectFileFilepath);
    }
}

MenuBar *MenuBar::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetMenuBar();
}

void MenuBar::OnNewScene(MenuItem*)
{
    MenuBar *mb = MenuBar::GetInstance();
    mb->m_sceneOpenerSaver->OnNewScene();
}

void MenuBar::OnSaveScene(MenuItem*)
{
    MenuBar *mb = MenuBar::GetInstance();
    mb->m_sceneOpenerSaver->OnSaveScene();
}

void MenuBar::OnSaveSceneAs(MenuItem*)
{
    MenuBar *mb = MenuBar::GetInstance();
    mb->m_sceneOpenerSaver->OnSaveSceneAs();
}

void MenuBar::OnOpenScene(MenuItem*)
{
    MenuBar *mb = MenuBar::GetInstance();
    mb->m_sceneOpenerSaver->OnOpenScene();
}

void MenuBar::OnBuild(MenuItem*)
{
    GameBuilder::BuildGame();
}

void MenuBar::OnBuildAndRun(MenuItem*)
{
    GameBuilder::BuildGame();

    class Runnable : public ThreadRunnable
    {
    public:
        String cmd = "";
        void Run() override { SystemUtils::System(cmd); }
    };

    Runnable *runnable = new Runnable();
    runnable->cmd = GameBuilder::GetExecutablePath().GetAbsolute();

    Thread *thread = new Thread();
    thread->SetRunnable(runnable);
    thread->Start();
}

template <class T>
T* OnAddComponent()
{
    GameObject *selectedGameObject = Editor::GetSelectedGameObject();
    if (selectedGameObject)
    {
        return selectedGameObject->AddComponent<T>();
    }
    return nullptr;
}

void MenuBar::OnAddAudioListener(MenuItem*)
{
    OnAddComponent<AudioListener>();
}

void MenuBar::OnAddAudioSource(MenuItem*)
{
    OnAddComponent<AudioSource>();
}

void MenuBar::OnAddNewBehaviour(MenuItem *item)
{
    Path behaviourDir = EditorPaths::GetProjectAssetsDir();
    String behaviourName = "";
    do
    {
        behaviourName = Dialog::GetString("Specify Behaviour name...",
                              "Please, the name of the new Behaviour: ",
                              "NewBehaviour");

        if (behaviourName == "") { return; }
    }
    while (!BehaviourCreator::CanCreateNewBehaviour(behaviourDir, behaviourName));

    Path behaviourHeaderPath;
    Path behaviourSourcePath;
    BehaviourCreator::CreateNewBehaviour(behaviourDir,
                                         behaviourName,
                                        &behaviourHeaderPath,
                                        &behaviourSourcePath);

    BehaviourContainer *behaviourContainer = OnAddComponent<BehaviourContainer>();
    behaviourContainer->SetSourceFilepath(behaviourSourcePath);
}

void MenuBar::OnAddEmptyBehaviour(MenuItem*)
{
    BehaviourContainer *behaviourContainer = OnAddComponent<BehaviourContainer>();
    (void) behaviourContainer;
}

void MenuBar::OnAddExistingBehaviour(MenuItem*)
{
    bool accepted;
    Path behaviourPath;
    EditorDialog::GetAsset("Select an existing Behaviour...",
                           Extensions::GetSourceFileExtensions(),
                           &behaviourPath,
                           &accepted);

    if (!accepted) { return; }

    BehaviourContainer *behaviourContainer = OnAddComponent<BehaviourContainer>();
    behaviourContainer->SetSourceFilepath(behaviourPath);
}

void MenuBar::OnAddCamera(MenuItem*)
{
    OnAddComponent<Camera>();
}

void MenuBar::OnAddPointLight(MenuItem*)
{
    OnAddComponent<PointLight>();
}

void MenuBar::OnAddDirectionalLight(MenuItem*)
{
    OnAddComponent<DirectionalLight>();
}

void MenuBar::OnAddLineRenderer(MenuItem*)
{
    OnAddComponent<LineRenderer>();
}

void MenuBar::OnAddMeshRenderer(MenuItem*)
{
    OnAddComponent<MeshRenderer>();
}

void MenuBar::OnAddTransform(MenuItem*)
{
    OnAddComponent<Transform>();
}

void MenuBar::OnAddRectTransform(MenuItem*)
{
    OnAddComponent<RectTransform>();
}

void MenuBar::OnCreateCone(MenuItem*)
{
    MenuBar::OnCreatePrimitive( GameObjectFactory::CreateConeGameObject() );
}

void MenuBar::OnCreateCube(MenuItem*)
{
    MenuBar::OnCreatePrimitive( GameObjectFactory::CreateCubeGameObject() );
}

void MenuBar::OnCreateSphere(MenuItem*)
{
    MenuBar::OnCreatePrimitive( GameObjectFactory::CreateSphereGameObject() );
}

void MenuBar::OnCreatePlane(MenuItem*)
{
    MenuBar::OnCreatePrimitive( GameObjectFactory::CreatePlaneGameObject() );
}

void MenuBar::OnCreatePrimitive(GameObject *primitive)
{
    Scene *openScene = EditorSceneManager::GetOpenScene();
    if (openScene)
    {
        primitive->SetParent(openScene);
        Editor::SelectGameObject(primitive);
    }
}

