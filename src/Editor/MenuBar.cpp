#include "BangEditor/MenuBar.h"

#include "Bang/Path.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Thread.h"
#include "Bang/Camera.h"
#include "Bang/Dialog.h"
#include "Bang/Random.h"
#include "Bang/UIMask.h"
#include "Bang/UILabel.h"
#include "Bang/Animator.h"
#include "Bang/Material.h"
#include "Bang/UIButton.h"
#include "Bang/UISlider.h"
#include "Bang/UICanvas.h"
#include "Bang/Behaviour.h"
#include "Bang/Resources.h"
#include "Bang/Transform.h"
#include "Bang/Extensions.h"
#include "Bang/IFocusable.h"
#include "Bang/PointLight.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIRectMask.h"
#include "Bang/SystemUtils.h"
#include "Bang/AudioSource.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIInputText.h"
#include "Bang/SceneManager.h"
#include "Bang/LineRenderer.h"
#include "Bang/MeshRenderer.h"
#include "Bang/AudioListener.h"
#include "Bang/RectTransform.h"
#include "Bang/UIAutoFocuser.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/WaterRenderer.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/DirectionalLight.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/PostProcessEffect.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/BehaviourContainer.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/SkinnedMeshRenderer.h"

#include "BangEditor/Editor.h"
#include "BangEditor/Explorer.h"
#include "BangEditor/MenuItem.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/GameBuilder.h"
#include "BangEditor/EditorDialog.h"
#include "BangEditor/EditorSettings.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/SceneOpenerSaver.h"
#include "BangEditor/BehaviourCreator.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/UndoRedoMoveGameObject.h"
#include "BangEditor/UndoRedoCreateGameObject.h"
#include "BangEditor/UndoRedoSerializableChange.h"

USING_NAMESPACE_BANG_EDITOR

MenuBar::MenuBar()
{
    SetName("MenuBar");
    GameObjectFactory::CreateUIGameObjectInto(this);

    p_focusable = AddComponent<UIFocusable>();

    UIImageRenderer *bg = AddComponent<UIImageRenderer>();
    bg->SetTint( Color::LightGray.WithValue(0.9f) );

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinHeight(15);

    m_horizontalLayout = AddComponent<UIHorizontalLayout>();
    m_horizontalLayout->SetSpacing(5);

    // File
    m_fileItem = AddItem();
    m_fileItem->GetText()->SetContent("File");
    MenuItem *newScene = m_fileItem->AddItem("New Scene");
    MenuItem *openScene = m_fileItem->AddItem("Open Scene");
    MenuItem *saveScene = m_fileItem->AddItem("Save Scene");
    MenuItem *saveSceneAs = m_fileItem->AddItem("Save Scene As...");
    m_fileItem->AddSeparator();
    MenuItem *build = m_fileItem->AddItem("Build");
    MenuItem *buildAndRun = m_fileItem->AddItem("Build and run");
    newScene->SetSelectedCallback(MenuBar::OnNewScene);
    saveScene->SetSelectedCallback(MenuBar::OnSaveScene);
    saveSceneAs->SetSelectedCallback(MenuBar::OnSaveSceneAs);
    openScene->SetSelectedCallback(MenuBar::OnOpenScene);
    build->SetSelectedCallback(MenuBar::OnBuild);
    buildAndRun->SetSelectedCallback(MenuBar::OnBuildAndRun);

    // Edit
    m_editItem = AddItem();
    m_editItem->GetText()->SetContent("Edit");
    MenuItem *editNone = m_editItem->AddItem("None");
    (void) editNone;

    // Assets
    m_assetsItem = AddItem();
    m_assetsItem->GetText()->SetContent("Assets");
    MenuItem *createMaterial = m_assetsItem->AddItem("Material");
    createMaterial->SetSelectedCallback(MenuBar::OnCreateMaterial);
    MenuItem *createTextureCubeMap = m_assetsItem->AddItem("Texture Cube Map");
    createTextureCubeMap->SetSelectedCallback(MenuBar::OnCreateTextureCubeMap);

    // Components
    m_componentsItem = AddItem();
    m_componentsItem->GetText()->SetContent("Components");
    MenuBar::CreateComponentsMenuInto(m_componentsItem);

    // GameObject
    m_gameObjectsItem = AddItem();
    m_gameObjectsItem->GetText()->SetContent("GameObjects");
    MenuItem *primitiveGameObjectItem = m_gameObjectsItem->AddItem("Primitives");
    MenuItem *createEmpty  = primitiveGameObjectItem->AddItem("Empty");
    MenuItem *createCone   = primitiveGameObjectItem->AddItem("Cone");
    MenuItem *createCube   = primitiveGameObjectItem->AddItem("Cube");
    MenuItem *createSphere = primitiveGameObjectItem->AddItem("Sphere");
    MenuItem *createPlane  = primitiveGameObjectItem->AddItem("Plane");
    MenuItem *lightsGOItem = m_gameObjectsItem->AddItem("Lights");
    MenuItem *createDirectionalLightGO = lightsGOItem->AddItem("Directional Light");
    MenuItem *createPointLightGO = lightsGOItem->AddItem("Point Light");
    MenuItem *uiItemGO = m_gameObjectsItem->AddItem("UI");
    MenuItem *createUIEmptyGO = uiItemGO->AddItem("Empty");
    MenuItem *createUICanvasGO = uiItemGO->AddItem("Canvas");
    MenuItem *createUITextGO = uiItemGO->AddItem("Text");
    MenuItem *createUIImageGO = uiItemGO->AddItem("Image");
    createEmpty->SetSelectedCallback(MenuBar::OnCreateEmpty);
    createCone->SetSelectedCallback(MenuBar::OnCreateCone);
    createCube->SetSelectedCallback(MenuBar::OnCreateCube);
    createPlane->SetSelectedCallback(MenuBar::OnCreatePlane);
    createSphere->SetSelectedCallback(MenuBar::OnCreateSphere);
    createDirectionalLightGO->SetSelectedCallback(MenuBar::OnCreateDirectionalLightGO);
    createPointLightGO->SetSelectedCallback(MenuBar::OnCreatePointLightGO);
    createUIEmptyGO->SetSelectedCallback(MenuBar::OnCreateUIEmptyGO);
    createUICanvasGO->SetSelectedCallback(MenuBar::OnCreateUICanvasGO);
    createUITextGO->SetSelectedCallback(MenuBar::OnCreateUITextGO);
    createUIImageGO->SetSelectedCallback(MenuBar::OnCreateUIImageGO);

    // Shortcuts
    RegisterShortcut( Shortcut(Key::LCTRL,              Key::S, "SaveScene")   );
    RegisterShortcut( Shortcut(Key::LCTRL, Key::LSHIFT, Key::S, "SaveSceneAs") );
    RegisterShortcut( Shortcut(Key::LCTRL,              Key::O, "OpenScene")   );
}

MenuBar::~MenuBar()
{
}

void MenuBar::RegisterShortcut(const Shortcut &shortcut)
{
    ShortcutManager::RegisterShortcut(shortcut, &MenuBar::OnShortcutPressed);
}

void MenuBar::OnShortcutPressed(const Shortcut &shortcut)
{
    if (shortcut.GetName() == "SaveScene") { OnSaveScene(nullptr); }
    if (shortcut.GetName() == "SaveSceneAs") { OnSaveSceneAs(nullptr); }
    if (shortcut.GetName() == "OpenScene") { OnOpenScene(nullptr); }
}

void MenuBar::Update()
{
    GameObject::Update();

    bool componentsEnabled = Editor::GetSelectedGameObject() != nullptr;
    m_componentsItem->SetOverAndActionEnabledRecursively(componentsEnabled);
    m_componentsItem->SetOverAndActionEnabled(true);

    UICanvas *canvas = UICanvas::GetActive(this);
    bool hasFocusRecursive = canvas->HasFocus(this, true);
    for (MenuItem *item : m_items)
    {
        // Drop down enabled if we have focus. Can't otherwise.
        item->SetDropDownEnabled(hasFocusRecursive);

        // Force show on top item if mouse over and menu bar has focus
        bool forceShow = (hasFocusRecursive && canvas->IsMouseOver(item, true));
        if (forceShow && item != p_currentTopItemBeingShown)
        {
            if (p_currentTopItemBeingShown) // Unforce show on the other
            {
                p_currentTopItemBeingShown->SetForceShow(false);
                p_currentTopItemBeingShown->Close(true);
            }

            item->SetForceShow(true); // Force show on item under mouse
            p_currentTopItemBeingShown = item;
        }
        if (p_currentTopItemBeingShown != item) { item->Close(true); }

        if (!hasFocusRecursive)
        {
            item->SetForceShow(false);
            item->Close(true);
        }
    }
    if (!hasFocusRecursive) { p_currentTopItemBeingShown = nullptr; }
}

MenuItem* MenuBar::AddItem()
{
    MenuItem *item = GameObject::Create<MenuItem>( MenuItem::MenuItemType::TOP );
    item->SetParent(this);
    m_items.PushBack(item);
    return item;
}

MenuItem* MenuBar::GetItem(int i)
{
    return m_items[i];
}

void MenuBar::CreateComponentsMenuInto(MenuItem *rootItem)
{
    MenuItem *addAudio = rootItem->AddItem("Audio");
    MenuItem *addAnimator = rootItem->AddItem("Animator");
    MenuItem *addAudioListener = addAudio->AddItem("Audio Listener");
    MenuItem *addAudioSource = addAudio->AddItem("Audio Source");
    MenuItem *addBehaviours = rootItem->AddItem("Behaviour");
    MenuItem *addNewBehaviour = addBehaviours->AddItem("New Behaviour...");
    MenuItem *addExistingBehaviour = addBehaviours->AddItem("Existing Behaviour...");
    MenuItem *addEmptyBehaviour = addBehaviours->AddItem("Empty Behaviour");
    MenuItem *addCamera = rootItem->AddItem("Camera");
    MenuItem *addLight = rootItem->AddItem("Light");
    MenuItem *addPointLight = addLight->AddItem("Point Light");
    MenuItem *addDirectionalLight = addLight->AddItem("Directional Light");
    MenuItem *addRenderer = rootItem->AddItem("Renderer");
    MenuItem *addLineRenderer = addRenderer->AddItem("LineRenderer");
    MenuItem *addMeshRenderer = addRenderer->AddItem("MeshRenderer");
    MenuItem *addSkinnedMeshRenderer = addRenderer->AddItem("SkinnedMeshRenderer");
    MenuItem *addWaterRenderer = addRenderer->AddItem("WaterRenderer");
    MenuItem *addTransforms = rootItem->AddItem("Transform");
    MenuItem *addTransform = addTransforms->AddItem("Transform");
    MenuItem *addRectTransform = addTransforms->AddItem("RectTransform");
    MenuItem *addPostProcessEffect = rootItem->AddItem("PostProcessEffect");
    MenuItem *addUI = rootItem->AddItem("UI");
    MenuItem *addUIAutoFocuser = addUI->AddItem("Auto Focuser");
    MenuItem *addUIButton = addUI->AddItem("Button");
    MenuItem *addUICanvas = addUI->AddItem("Canvas");
    MenuItem *addUICheckBox = addUI->AddItem("CheckBox");
    MenuItem *addUIComboBox = addUI->AddItem("ComboBox");
    MenuItem *addUIFocusable = addUI->AddItem("Focusable");
    MenuItem *addUIHorizontalLayout = addUI->AddItem("HorizontalLayout");
    MenuItem *addUIImageRenderer = addUI->AddItem("Image Renderer");
    MenuItem *addUIInputText = addUI->AddItem("InputText");
    MenuItem *addUIMask = addUI->AddItem("Mask");
    MenuItem *addUILabel = addUI->AddItem("Label");
    MenuItem *addUILayoutIgnorer = addUI->AddItem("LayoutIgnorer");
    MenuItem *addUILayoutElement = addUI->AddItem("LayoutElement");
    MenuItem *addUIRectMask = addUI->AddItem("RectMask");
    MenuItem *addUISlider = addUI->AddItem("Slider");
    MenuItem *addUIScrollPanel = addUI->AddItem("ScrollPanel");
    MenuItem *addUITextRenderer = addUI->AddItem("Text Renderer");
    MenuItem *addUIVerticalLayout = addUI->AddItem("VerticalLayout");
    addAnimator->SetSelectedCallback(MenuBar::OnAddAnimator);
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
    addSkinnedMeshRenderer->SetSelectedCallback(MenuBar::OnAddSkinnedMeshRenderer);
    addWaterRenderer->SetSelectedCallback(MenuBar::OnAddWaterRenderer);
    addTransform->SetSelectedCallback(MenuBar::OnAddTransform);
    addRectTransform->SetSelectedCallback(MenuBar::OnAddRectTransform);
    addPostProcessEffect->SetSelectedCallback(MenuBar::OnAddPostProcessEffect);
    addUICanvas->SetSelectedCallback(MenuBar::OnAddUICanvas);
    addUIImageRenderer->SetSelectedCallback(MenuBar::OnAddUIImageRenderer);
    addUITextRenderer->SetSelectedCallback(MenuBar::OnAddUITextRenderer);
    addUIFocusable->SetSelectedCallback(MenuBar::OnAddUIFocusable);
    addUILayoutIgnorer->SetSelectedCallback(MenuBar::OnAddUILayoutIgnorer);
    addUILayoutElement->SetSelectedCallback(MenuBar::OnAddUILayoutElement);
    addUIButton->SetSelectedCallback(MenuBar::OnAddUIButton);
    addUILabel->SetSelectedCallback(MenuBar::OnAddUILabel);
    addUIInputText->SetSelectedCallback(MenuBar::OnAddUIInputText);
    addUIMask->SetSelectedCallback(MenuBar::OnAddUIMask);
    addUIRectMask->SetSelectedCallback(MenuBar::OnAddUIRectMask);
    addUICheckBox->SetSelectedCallback(MenuBar::OnAddUICheckBox);
    addUIComboBox->SetSelectedCallback(MenuBar::OnAddUIComboBox);
    addUIHorizontalLayout->SetSelectedCallback(MenuBar::OnAddUIHorizontalLayout);
    addUIVerticalLayout->SetSelectedCallback(MenuBar::OnAddUIVerticalLayout);
    addUISlider->SetSelectedCallback(MenuBar::OnAddUISlider);
    addUIScrollPanel->SetSelectedCallback(MenuBar::OnAddUIScrollPanel);
    addUIAutoFocuser->SetSelectedCallback(MenuBar::OnAddUIAutoFocuser);
}

MenuBar *MenuBar::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetMenuBar();
}

void MenuBar::OnNewScene(MenuItem*)
{
    SceneOpenerSaver::GetInstance()->OnNewScene();
}

void MenuBar::OnSaveScene(MenuItem*)
{
    SceneOpenerSaver::GetInstance()->OnSaveScene();
}

void MenuBar::OnSaveSceneAs(MenuItem*)
{
    SceneOpenerSaver::GetInstance()->OnSaveSceneAs();
}

void MenuBar::OnOpenScene(MenuItem*)
{
    SceneOpenerSaver::GetInstance()->OnOpenScene();
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


void AfterCreateAssetFile(const Path &createdAssetPath)
{
    Explorer::GetInstance()->ForceCheckFileChanges();
    Explorer::GetInstance()->SelectPath(createdAssetPath);
}

template <class T>
std::pair<RH<T>, Path>
OnCreateAssetFile(const String &name, const String &extension)
{
    Path currentPath = Explorer::GetInstance()->GetCurrentPath();
    Path assetPath = currentPath.Append("New_" + name)
                    .AppendExtension(extension).GetDuplicatePath();
    RH<T> asset = Resources::Create<T>();
    Resources::CreateResourceXMLAndImportFile(asset.Get(), assetPath);
    AfterCreateAssetFile(assetPath);
    return std::make_pair(asset, assetPath);
}

void MenuBar::OnCreateMaterial(MenuItem*)
{
    OnCreateAssetFile<Material>("Material", Extensions::GetMaterialExtension());
}

void MenuBar::OnCreateTextureCubeMap(MenuItem *item)
{
    OnCreateAssetFile<TextureCubeMap>("CubeMap", Extensions::GetTextureCubeMapExtension());
}

template <class T>
T* OnAddComponent()
{
    T *comp = nullptr;
    GameObject *selectedGameObject = Editor::GetSelectedGameObject();
    if (selectedGameObject)
    {
        XMLNode undoXMLBefore = selectedGameObject->GetXMLInfo();

        comp = selectedGameObject->AddComponent<T>();

        XMLNode currentXML = selectedGameObject->GetXMLInfo();
        UndoRedoManager::PushAction(
                    new UndoRedoSerializableChange(selectedGameObject,
                                                   undoXMLBefore, currentXML));
    }

    return comp;
}

void MenuBar::OnAddAnimator(MenuItem *item)
{
    OnAddComponent<Animator>();
}

void MenuBar::OnAddAudioListener(MenuItem*)
{
    OnAddComponent<AudioListener>();
}

void MenuBar::OnAddAudioSource(MenuItem*)
{
    OnAddComponent<AudioSource>();
}

void MenuBar::OnAddNewBehaviour(MenuItem*)
{
    Path behaviourDir = Paths::GetProjectAssetsDir();
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

void MenuBar::OnAddSkinnedMeshRenderer(MenuItem *item)
{
    OnAddComponent<SkinnedMeshRenderer>();
}

void MenuBar::OnAddWaterRenderer(MenuItem *item)
{
    OnAddComponent<WaterRenderer>();
}

void MenuBar::OnAddTransform(MenuItem*)
{
    OnAddComponent<Transform>();
}

void MenuBar::OnAddRectTransform(MenuItem*)
{
    OnAddComponent<RectTransform>();
}

void MenuBar::OnAddUIAutoFocuser(MenuItem*)
{
    OnAddComponent<UIAutoFocuser>();
}

void MenuBar::OnAddUIButton(MenuItem*)
{
    OnAddComponent<UIButton>();
}

void MenuBar::OnAddUIHorizontalLayout(MenuItem*)
{
    OnAddComponent<UIHorizontalLayout>();
}

void MenuBar::OnAddUIVerticalLayout(MenuItem*)
{
    OnAddComponent<UIVerticalLayout>();
}

void MenuBar::OnAddUILayoutElement(MenuItem*)
{
    OnAddComponent<UILayoutElement>();
}

void MenuBar::OnAddUILayoutIgnorer(MenuItem*)
{
    OnAddComponent<UILayoutIgnorer>();
}

void MenuBar::OnAddUIMask(MenuItem*)
{
    OnAddComponent<UIMask>();
}

void MenuBar::OnAddUIRectMask(MenuItem*)
{
    OnAddComponent<UIRectMask>();
}

void MenuBar::OnAddUISlider(MenuItem*)
{
    OnAddComponent<UISlider>();
}

void MenuBar::OnAddUIScrollPanel(MenuItem*)
{
    OnAddComponent<UIScrollPanel>();
}

void MenuBar::OnAddUITextRenderer(MenuItem*)
{
    OnAddComponent<UITextRenderer>();
}

void MenuBar::OnAddUIImageRenderer(MenuItem*)
{
    OnAddComponent<UIImageRenderer>();
}

void MenuBar::OnAddUICanvas(MenuItem*)
{
    OnAddComponent<UICanvas>();
}

void MenuBar::OnAddUICheckBox(MenuItem*)
{
    OnAddComponent<UICheckBox>();
}

void MenuBar::OnAddUIComboBox(MenuItem*)
{
    OnAddComponent<UIComboBox>();
}

void MenuBar::OnAddUIFocusable(MenuItem*)
{
    OnAddComponent<UIFocusable>();
}

void MenuBar::OnAddUIInputText(MenuItem*)
{
    OnAddComponent<UIInputText>();
}

void MenuBar::OnAddUILabel(MenuItem*)
{
    OnAddComponent<UILabel>();
}

void MenuBar::OnAddPostProcessEffect(MenuItem*)
{
    OnAddComponent<PostProcessEffect>();
}

void MenuBar::OnCreateEmpty(MenuItem *)
{
    GameObject *go = GameObjectFactory::CreateGameObject();
    go->SetName("Empty");
    MenuBar::OnEndCreateGameObjectFromMenuBar(go);
}

void MenuBar::OnCreateCone(MenuItem*)
{
    MenuBar::OnEndCreateGameObjectFromMenuBar(
                GameObjectFactory::CreateConeGameObject() );
}

void MenuBar::OnCreateCube(MenuItem*)
{
    MenuBar::OnEndCreateGameObjectFromMenuBar(
                GameObjectFactory::CreateCubeGameObject() );
}

void MenuBar::OnCreateSphere(MenuItem*)
{
    MenuBar::OnEndCreateGameObjectFromMenuBar(
                GameObjectFactory::CreateSphereGameObject() );
}

void MenuBar::OnCreateDirectionalLightGO(MenuItem*)
{
    GameObject *dlGo = GameObjectFactory::CreateGameObject();
    dlGo->AddComponent<DirectionalLight>();
    MenuBar::OnEndCreateGameObjectFromMenuBar(dlGo);
}

void MenuBar::OnCreatePointLightGO(MenuItem*)
{
    GameObject *plGo = GameObjectFactory::CreateGameObject();
    plGo->AddComponent<PointLight>();
    MenuBar::OnEndCreateGameObjectFromMenuBar(plGo);
}

UICanvas* MenuBar::OnCreateUICanvasGO(MenuItem*)
{
    GameObject *uiGo = GameObjectFactory::CreateUIGameObject();
    uiGo->SetName("Canvas");
    UICanvas *canvas = uiGo->AddComponent<UICanvas>();
    MenuBar::OnEndCreateUIGameObjectFromMenuBar(uiGo);
    return canvas;
}

void MenuBar::OnCreateUIEmptyGO(MenuItem*)
{
    GameObject *uiGo = GameObjectFactory::CreateUIGameObject();
    uiGo->SetName("EmptyUI");
    MenuBar::OnEndCreateUIGameObjectFromMenuBar(uiGo);
}

void MenuBar::OnCreateUIImageGO(MenuItem*)
{
    GameObject *uiGo = GameObjectFactory::CreateUIGameObject();
    uiGo->SetName("Image");
    uiGo->AddComponent<UIImageRenderer>();
    MenuBar::OnEndCreateUIGameObjectFromMenuBar(uiGo);
}

void MenuBar::OnCreateUITextGO(MenuItem*)
{
    GameObject *uiGo = GameObjectFactory::CreateUIGameObject();
    uiGo->SetName("Text");
    UITextRenderer *text = uiGo->AddComponent<UITextRenderer>();
    text->SetContent("UIText");
    MenuBar::OnEndCreateUIGameObjectFromMenuBar(uiGo);
}

void MenuBar::OnCreatePlane(MenuItem*)
{
    MenuBar::OnEndCreateGameObjectFromMenuBar(
                GameObjectFactory::CreatePlaneGameObject() );
}

void MenuBar::OnEndCreateGameObjectFromMenuBar(GameObject *go)
{
    GameObject *parentGo = Editor::GetSelectedGameObject();
    if (!parentGo) { parentGo = EditorSceneManager::GetOpenScene(); }
    if (parentGo)
    {
        go->SetParent(parentGo);

        UndoRedoManager::PushAction( new UndoRedoCreateGameObject(go) );

        Editor::GetInstance()->SelectGameObject(go, false);
    }
}

void MenuBar::OnEndCreateUIGameObjectFromMenuBar(GameObject *uiGo)
{
    GameObject *parentGo = Editor::GetSelectedGameObject();
    if (!parentGo) { parentGo = EditorSceneManager::GetOpenScene(); }
    if (parentGo)
    {
        uiGo->SetParent(parentGo);

        if (!UICanvas::GetActive(uiGo))
        {
            UICanvas *createdCanvas = OnCreateUICanvasGO(nullptr);
            GameObject *createdCanvasGo = createdCanvas->GetGameObject();
            uiGo->SetParent( createdCanvasGo );

            UndoRedoManager::PushAction( new UndoRedoCreateGameObject(uiGo) );
        }
        Editor::GetInstance()->SelectGameObject(uiGo, false);
    }
}

