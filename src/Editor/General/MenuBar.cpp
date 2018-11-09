#include "BangEditor/MenuBar.h"

#include <utility>

#include "Bang/Animator.h"
#include "Bang/AnimatorLayerMask.h"
#include "Bang/AnimatorStateMachine.h"
#include "Bang/Array.tcc"
#include "Bang/AudioListener.h"
#include "Bang/AudioSource.h"
#include "Bang/BehaviourContainer.h"
#include "Bang/BoxCollider.h"
#include "Bang/Camera.h"
#include "Bang/CapsuleCollider.h"
#include "Bang/Cloth.h"
#include "Bang/Color.h"
#include "Bang/Dialog.h"
#include "Bang/DirectionalLight.h"
#include "Bang/Extensions.h"
#include "Bang/Flags.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Key.h"
#include "Bang/LineRenderer.h"
#include "Bang/Material.h"
#include "Bang/MeshCollider.h"
#include "Bang/MeshRenderer.h"
#include "Bang/MetaNode.h"
#include "Bang/ParticleSystem.h"
#include "Bang/Path.h"
#include "Bang/PhysicsMaterial.h"
#include "Bang/PointLight.h"
#include "Bang/PostProcessEffect.h"
#include "Bang/PostProcessEffectSSAO.h"
#include "Bang/RectTransform.h"
#include "Bang/ReflectionProbe.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
#include "Bang/RigidBody.h"
#include "Bang/Rope.h"
#include "Bang/Scene.h"
#include "Bang/SkinnedMeshRenderer.h"
#include "Bang/SphereCollider.h"
#include "Bang/SystemUtils.h"
#include "Bang/TextureCubeMap.h"
#include "Bang/Thread.h"
#include "Bang/Transform.h"
#include "Bang/UIAutoFocuser.h"
#include "Bang/UIButton.h"
#include "Bang/UICanvas.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIInputText.h"
#include "Bang/UILabel.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UIMask.h"
#include "Bang/UIRectMask.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UISlider.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/WaterRenderer.h"
#include "BangEditor/BehaviourCreator.h"
#include "BangEditor/EditSceneGameObjects.h"
#include "BangEditor/Editor.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorDialog.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/Explorer.h"
#include "BangEditor/GameBuilder.h"
#include "BangEditor/Inspector.h"
#include "BangEditor/MenuItem.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/SIWEditorSettings.h"
#include "BangEditor/SIWPhysicsSettings.h"
#include "BangEditor/SIWRenderSettings.h"
#include "BangEditor/SceneOpenerSaver.h"
#include "BangEditor/Shortcut.h"
#include "BangEditor/ShortcutManager.h"
#include "BangEditor/UndoRedoCreateGameObject.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoSerializableChange.h"

using namespace BangEditor;

MenuBar::MenuBar()
{
    SetName("MenuBar");
    GameObjectFactory::CreateUIGameObjectInto(this);
    GameObjectFactory::AddInnerBorder(this);
    GameObjectFactory::AddOuterShadow(this, Vector2i(0, 5), 0.3f);

    p_focusable = AddComponent<UIFocusable>();

    UIImageRenderer *bg = AddComponent<UIImageRenderer>();
    bg->SetTint(Color::White().WithValue(0.85f));

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinHeight(20);

    m_horizontalLayout = AddComponent<UIHorizontalLayout>();
    m_horizontalLayout->SetSpacing(10);

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
    m_undoItem = m_editItem->AddItem("Undo - Ctrl + Z");
    m_redoItem = m_editItem->AddItem("Redo - Ctrl + Shift + Z");
    m_editItem->AddSeparator();
    MenuItem *editorSettingsItem = m_editItem->AddItem("Editor Settings");
    m_projectSettingsItem = m_editItem->AddItem("Project Settings");
    m_undoItem->SetSelectedCallback(MenuBar::OnUndo);
    m_redoItem->SetSelectedCallback(MenuBar::OnRedo);
    MenuItem *renderSettings =
        m_projectSettingsItem->AddItem("Render Settings");
    MenuItem *physicsSettings =
        m_projectSettingsItem->AddItem("Physics Settings");
    editorSettingsItem->SetSelectedCallback(MenuBar::OnEditorSettings);
    renderSettings->SetSelectedCallback(MenuBar::OnRenderSettings);
    physicsSettings->SetSelectedCallback(MenuBar::OnPhysicsSettings);

    // Assets
    m_assetsItem = AddItem();
    m_assetsItem->GetText()->SetContent("Assets");
    MenuBar::CreateAssetsMenuInto(m_assetsItem);

    // Components
    m_componentsItem = AddItem();
    m_componentsItem->GetText()->SetContent("Components");
    MenuBar::CreateComponentsMenuInto(m_componentsItem);

    // GameObject
    m_gameObjectsItem = AddItem();
    m_gameObjectsItem->GetText()->SetContent("GameObjects");
    CreateGameObjectCreateMenuInto(m_gameObjectsItem);
    m_gameObjectsItem->AddSeparator();
    CreateGameObjectMiscMenuInto(m_gameObjectsItem,
                                 &m_alignGameObjectWithViewItem,
                                 &m_alignViewWithGameObjectItem);

    // Shortcuts
    RegisterShortcut(Shortcut(Key::S, KeyModifier::LCTRL, "SaveScene"));
    RegisterShortcut(Shortcut(
        Key::S, (KeyModifier::LCTRL | KeyModifier::LSHIFT), "SaveSceneAs"));
    RegisterShortcut(Shortcut(Key::O, KeyModifier::LCTRL, "OpenScene"));
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
    if (shortcut.GetName() == "SaveScene")
    {
        OnSaveScene(nullptr);
    }
    else if (shortcut.GetName() == "SaveSceneAs")
    {
        OnSaveSceneAs(nullptr);
    }
    else if (shortcut.GetName() == "OpenScene")
    {
        OnOpenScene(nullptr);
    }
}

void MenuBar::Update()
{
    GameObject::Update();

    // Force show
    UICanvas *canvas = UICanvas::GetActive(this);
    bool menuBarHasFocus = false;
    for (MenuItem *item : m_items)
    {
        if (canvas->HasFocus(item->GetFocusable()))
        {
            menuBarHasFocus = true;
            break;
        }
    }

    for (MenuItem *item : m_items)
    {
        // Drop down enabled if we have focus. Can't otherwise.
        item->SetDropDownEnabled(menuBarHasFocus);

        // Force show on top item if mouse over and menu bar has focus
        bool needToShowThisItem =
            (menuBarHasFocus && canvas->IsMouseOver(item, true));
        if (needToShowThisItem && item != p_currentTopItemBeingShown)
        {
            if (p_currentTopItemBeingShown)  // Unforce show on the other
            {
                p_currentTopItemBeingShown->SetForceShow(false);
                p_currentTopItemBeingShown->Close(true);
            }

            item->SetForceShow(true);  // Force show on item under mouse
            p_currentTopItemBeingShown = item;
        }

        if (item != p_currentTopItemBeingShown)
        {
            item->Close(true);
        }

        if (!menuBarHasFocus)
        {
            item->SetForceShow(false);
            item->Close(true);
        }
    }

    if (!menuBarHasFocus)
    {
        p_currentTopItemBeingShown = nullptr;
    }

    // Enabled/Disabled menu items
    bool componentsEnabled = (Editor::GetSelectedGameObject() != nullptr);
    m_componentsItem->SetOverAndActionEnabledRecursively(componentsEnabled);
    m_componentsItem->SetOverAndActionEnabled(true);
    m_undoItem->SetOverAndActionEnabled(UndoRedoManager::CanUndo());
    m_redoItem->SetOverAndActionEnabled(UndoRedoManager::CanRedo());
    m_projectSettingsItem->SetOverAndActionEnabledRecursively(
        (ProjectManager::GetCurrentProject() != nullptr));

    bool thereIsGameObjectSelected = (Editor::GetSelectedGameObject());
    m_alignGameObjectWithViewItem->SetOverAndActionEnabled(
        thereIsGameObjectSelected);
    m_alignViewWithGameObjectItem->SetOverAndActionEnabled(
        thereIsGameObjectSelected);
}

MenuItem *MenuBar::AddItem()
{
    MenuItem *item = new MenuItem(MenuItem::MenuItemType::TOP);
    item->SetParent(this);
    m_items.PushBack(item);
    return item;
}

MenuItem *MenuBar::GetItem(int i)
{
    return m_items[i];
}

void MenuBar::CreateGameObjectCreateMenuInto(MenuItem *rootItem)
{
    MenuItem *createEmpty = rootItem->AddItem("Empty");
    MenuItem *primitiveGameObjectItem = rootItem->AddItem("Primitives");
    MenuItem *createCone = primitiveGameObjectItem->AddItem("Cone");
    MenuItem *createCube = primitiveGameObjectItem->AddItem("Cube");
    MenuItem *createCapsule = primitiveGameObjectItem->AddItem("Capsule");
    MenuItem *createSphere = primitiveGameObjectItem->AddItem("Sphere");
    MenuItem *createCylinder = primitiveGameObjectItem->AddItem("Cylinder");
    MenuItem *createPlane = primitiveGameObjectItem->AddItem("Plane");
    MenuItem *createCam = rootItem->AddItem("Camera");
    MenuItem *createRendering = rootItem->AddItem("Rendering");
    MenuItem *createParticleSystemGO =
        createRendering->AddItem("Particle System");
    MenuItem *lightsGOItem = rootItem->AddItem("Lights");
    MenuItem *createDirectionalLightGO =
        lightsGOItem->AddItem("Directional Light");
    MenuItem *createPointLightGO = lightsGOItem->AddItem("Point Light");
    MenuItem *uiItemGO = rootItem->AddItem("UI");
    MenuItem *createUIEmptyGO = uiItemGO->AddItem("Empty");
    MenuItem *createUICanvasGO = uiItemGO->AddItem("Canvas");
    MenuItem *createUITextGO = uiItemGO->AddItem("Text");
    MenuItem *createUIImageGO = uiItemGO->AddItem("Image");
    createEmpty->SetSelectedCallback(MenuBar::OnCreateEmpty);
    createCone->SetSelectedCallback(MenuBar::OnCreateCone);
    createCube->SetSelectedCallback(MenuBar::OnCreateCube);
    createCapsule->SetSelectedCallback(MenuBar::OnCreateCapsule);
    createPlane->SetSelectedCallback(MenuBar::OnCreatePlane);
    createSphere->SetSelectedCallback(MenuBar::OnCreateSphere);
    createCylinder->SetSelectedCallback(MenuBar::OnCreateCylinder);
    createCam->SetSelectedCallback(MenuBar::OnCreateCamera);
    createParticleSystemGO->SetSelectedCallback(
        MenuBar::OnCreateParticleSystemGO);
    createDirectionalLightGO->SetSelectedCallback(
        MenuBar::OnCreateDirectionalLightGO);
    createPointLightGO->SetSelectedCallback(MenuBar::OnCreatePointLightGO);
    createUIEmptyGO->SetSelectedCallback(MenuBar::OnCreateUIEmptyGO);
    createUICanvasGO->SetSelectedCallback(MenuBar::OnCreateUICanvasGO);
    createUITextGO->SetSelectedCallback(MenuBar::OnCreateUITextGO);
    createUIImageGO->SetSelectedCallback(MenuBar::OnCreateUIImageGO);
}

void MenuBar::CreateGameObjectMiscMenuInto(
    MenuItem *rootItem,
    MenuItem **alignGameObjectWithViewItemOut,
    MenuItem **alignViewWithGameObjectItemOut)
{
    MenuItem *alignGameObjectWithViewItem =
        rootItem->AddItem("Align GameObject with view");
    MenuItem *alignViewWithGameObjectItem =
        rootItem->AddItem("Align view with GameObject");
    alignGameObjectWithViewItem->SetSelectedCallback(
        MenuBar::OnAlignGameObjectWithView);
    alignViewWithGameObjectItem->SetSelectedCallback(
        MenuBar::OnAlignViewWithGameObject);

    if (alignGameObjectWithViewItemOut)
    {
        *alignGameObjectWithViewItemOut = alignGameObjectWithViewItem;
    }

    if (alignViewWithGameObjectItemOut)
    {
        *alignViewWithGameObjectItemOut = alignViewWithGameObjectItem;
    }
}

void MenuBar::CreateComponentsMenuInto(MenuItem *rootItem)
{
    MenuItem *addAudio = rootItem->AddItem("Audio");
    MenuItem *addAnimator = rootItem->AddItem("Animator");
    MenuItem *addAudioListener = addAudio->AddItem("Audio Listener");
    MenuItem *addAudioSource = addAudio->AddItem("Audio Source");
    MenuItem *addBehaviours = rootItem->AddItem("Behaviour");
    MenuItem *addNewBehaviour = addBehaviours->AddItem("New Behaviour...");
    MenuItem *addExistingBehaviour =
        addBehaviours->AddItem("Existing Behaviour...");
    MenuItem *addEmptyBehaviour = addBehaviours->AddItem("Empty Behaviour");
    MenuItem *addCamera = rootItem->AddItem("Camera");
    MenuItem *addLight = rootItem->AddItem("Light");
    MenuItem *addPointLight = addLight->AddItem("Point Light");
    MenuItem *addDirectionalLight = addLight->AddItem("Directional Light");
    MenuItem *addRenderer = rootItem->AddItem("Renderer");
    MenuItem *addLineRenderer = addRenderer->AddItem("LineRenderer");
    MenuItem *addMeshRenderer = addRenderer->AddItem("MeshRenderer");
    MenuItem *addSkinnedMeshRenderer =
        addRenderer->AddItem("SkinnedMeshRenderer");
    MenuItem *addWaterRenderer = addRenderer->AddItem("WaterRenderer");
    MenuItem *addParticleSystem = addRenderer->AddItem("ParticleSystem");
    MenuItem *addTransforms = rootItem->AddItem("Transform");
    MenuItem *addTransform = addTransforms->AddItem("Transform");
    MenuItem *addRectTransform = addTransforms->AddItem("RectTransform");
    MenuItem *addPhysics = rootItem->AddItem("Physics");
    MenuItem *addRigidBody = addPhysics->AddItem("RigidBody");
    MenuItem *addColliders = addPhysics->AddItem("Colliders");
    MenuItem *addBoxCollider = addColliders->AddItem("BoxCollider");
    MenuItem *addSphereCollider = addColliders->AddItem("SphereCollider");
    MenuItem *addCapsuleCollider = addColliders->AddItem("CapsuleCollider");
    MenuItem *addMeshCollider = addColliders->AddItem("MeshCollider");
    MenuItem *addRope = addPhysics->AddItem("Rope");
    MenuItem *addCloth = addPhysics->AddItem("Cloth");
    MenuItem *addPostProcessEffects = rootItem->AddItem("Post Process Effects");
    MenuItem *addPostProcessEffect =
        addPostProcessEffects->AddItem("PostProcessEffect");
    MenuItem *addSSAO = addPostProcessEffects->AddItem("SSAO");
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
    MenuItem *addMisc = rootItem->AddItem("Misc");
    MenuItem *addReflectionProbe = addMisc->AddItem("ReflectionProbe");
    addBoxCollider->SetSelectedCallback(MenuBar::OnAddBoxCollider);
    addCapsuleCollider->SetSelectedCallback(MenuBar::OnAddCapsuleCollider);
    addSphereCollider->SetSelectedCallback(MenuBar::OnAddSphereCollider);
    addMeshCollider->SetSelectedCallback(MenuBar::OnAddMeshCollider);
    addRope->SetSelectedCallback(MenuBar::OnAddRope);
    addCloth->SetSelectedCallback(MenuBar::OnAddCloth);
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
    addSkinnedMeshRenderer->SetSelectedCallback(
        MenuBar::OnAddSkinnedMeshRenderer);
    addWaterRenderer->SetSelectedCallback(MenuBar::OnAddWaterRenderer);
    addParticleSystem->SetSelectedCallback(MenuBar::OnAddParticleSystem);
    addReflectionProbe->SetSelectedCallback(MenuBar::OnAddReflectionProbe);
    addTransform->SetSelectedCallback(MenuBar::OnAddTransform);
    addRigidBody->SetSelectedCallback(MenuBar::OnAddRigidBody);
    addRectTransform->SetSelectedCallback(MenuBar::OnAddRectTransform);
    addPostProcessEffect->SetSelectedCallback(MenuBar::OnAddPostProcessEffect);
    addSSAO->SetSelectedCallback(MenuBar::OnAddSSAO);
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
    addUIHorizontalLayout->SetSelectedCallback(
        MenuBar::OnAddUIHorizontalLayout);
    addUIVerticalLayout->SetSelectedCallback(MenuBar::OnAddUIVerticalLayout);
    addUISlider->SetSelectedCallback(MenuBar::OnAddUISlider);
    addUIScrollPanel->SetSelectedCallback(MenuBar::OnAddUIScrollPanel);
    addUIAutoFocuser->SetSelectedCallback(MenuBar::OnAddUIAutoFocuser);
}

void BangEditor::MenuBar::CreateAssetsMenuInto(BangEditor::MenuItem *rootItem)
{
    MenuItem *createMaterial = rootItem->AddItem("Material");
    MenuItem *createPhysicsMaterial = rootItem->AddItem("Physics Material");
    MenuItem *createAnimatorSM = rootItem->AddItem("Animator State Machine");
    MenuItem *createAnimatorLayerMask =
        rootItem->AddItem("Animator Layer Mask");
    MenuItem *createBehaviour = rootItem->AddItem("Behaviour");
    MenuItem *createTextureCubeMap = rootItem->AddItem("Texture Cube Map");
    createMaterial->SetSelectedCallback(MenuBar::OnCreateMaterial);
    createAnimatorSM->SetSelectedCallback(
        MenuBar::OnCreateAnimatorStateMachine);
    createAnimatorLayerMask->SetSelectedCallback(
        MenuBar::OnCreateAnimatorLayerMask);
    createPhysicsMaterial->SetSelectedCallback(
        MenuBar::OnCreatePhysicsMaterial);
    createBehaviour->SetSelectedCallback(MenuBar::OnCreateBehaviour);
    createTextureCubeMap->SetSelectedCallback(MenuBar::OnCreateTextureCubeMap);
}

MenuBar *MenuBar::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetMenuBar();
}

void MenuBar::OnNewScene(MenuItem *)
{
    SceneOpenerSaver::GetInstance()->OnNewScene();
}

void MenuBar::OnSaveScene(MenuItem *)
{
    SceneOpenerSaver::GetInstance()->OnSaveScene();
}

void MenuBar::OnSaveSceneAs(MenuItem *)
{
    SceneOpenerSaver::GetInstance()->OnSaveSceneAs();
}

void MenuBar::OnOpenScene(MenuItem *)
{
    SceneOpenerSaver::GetInstance()->OnOpenScene();
}

void MenuBar::OnBuild(MenuItem *)
{
    GameBuilder::BuildGame();
}

void MenuBar::OnBuildAndRun(MenuItem *)
{
    GameBuilder::BuildGame();

    class Runnable : public ThreadRunnable
    {
    public:
        String cmd = "";
        void Run() override
        {
            SystemUtils::System(cmd);
        }
    };

    Runnable *runnable = new Runnable();
    runnable->cmd = GameBuilder::GetExecutablePath().GetAbsolute();

    Thread *thread = new Thread();
    thread->SetRunnable(runnable);
    thread->Start();
}

void MenuBar::OnUndo(MenuItem *)
{
    UndoRedoManager::Undo();
}

void MenuBar::OnRedo(MenuItem *)
{
    UndoRedoManager::Redo();
}

void MenuBar::OnEditorSettings(MenuItem *)
{
    SIWEditorSettings *siw = new SIWEditorSettings();
    siw->Init();
    Inspector::GetActive()->ShowInspectorWidget(siw);
}

void MenuBar::OnRenderSettings(MenuItem *)
{
    SIWRenderSettings *siw = new SIWRenderSettings();
    siw->Init();
    Inspector::GetActive()->ShowInspectorWidget(siw);
}

void MenuBar::OnPhysicsSettings(MenuItem *)
{
    SIWPhysicsSettings *siw = new SIWPhysicsSettings();
    siw->Init();
    Inspector::GetActive()->ShowInspectorWidget(siw);
}

void AfterCreateAssetFile(const Path &createdAssetPath)
{
    Explorer::GetInstance()->ForceCheckFileChanges();
    Explorer::GetInstance()->SelectPath(createdAssetPath);
}

template <class T>
std::pair<RH<T>, Path> OnCreateAssetFile(const String &name,
                                         const String &extension)
{
    Path currentPath = Explorer::GetInstance()->GetCurrentPath();
    Path assetPath = currentPath.Append("New_" + name)
                         .AppendExtension(extension)
                         .GetDuplicatePath();
    RH<T> asset = Resources::Create<T>();
    Resources::CreateResourceMetaAndImportFile(asset.Get(), assetPath);
    AfterCreateAssetFile(assetPath);
    return std::make_pair(asset, assetPath);
}

void MenuBar::OnCreateMaterial(MenuItem *)
{
    OnCreateAssetFile<Material>("Material", Extensions::GetMaterialExtension());
}

void MenuBar::OnCreateBehaviour(MenuItem *)
{
    CreateNewBehaviour();
}

void MenuBar::OnCreatePhysicsMaterial(MenuItem *)
{
    OnCreateAssetFile<PhysicsMaterial>(
        "PhysicsMaterial", Extensions::GetPhysicsMaterialExtension());
}

void MenuBar::OnCreateAnimatorLayerMask(MenuItem *)
{
    OnCreateAssetFile<AnimatorStateMachine>(
        "AnimatorLayerMask", Extensions::GetAnimatorLayerMaskExtension());
}

void MenuBar::OnCreateAnimatorStateMachine(MenuItem *)
{
    OnCreateAssetFile<AnimatorStateMachine>(
        "AnimatorSM", Extensions::GetAnimatorStateMachineExtension());
}

void MenuBar::OnCreateTextureCubeMap(MenuItem *)
{
    OnCreateAssetFile<TextureCubeMap>("CubeMap",
                                      Extensions::GetTextureCubeMapExtension());
}

template <class T>
T *OnAddComponent()
{
    T *comp = nullptr;
    GameObject *selectedGameObject = Editor::GetSelectedGameObject();
    if (selectedGameObject)
    {
        MetaNode undoMetaBefore = selectedGameObject->GetMeta();

        comp = selectedGameObject->AddComponent<T>();

        MetaNode currentMeta = selectedGameObject->GetMeta();
        UndoRedoManager::PushAction(new UndoRedoSerializableChange(
            selectedGameObject, undoMetaBefore, currentMeta));
    }

    return comp;
}

void MenuBar::OnAddAnimator(MenuItem *)
{
    OnAddComponent<Animator>();
}

void MenuBar::OnAddAudioListener(MenuItem *)
{
    OnAddComponent<AudioListener>();
}

void MenuBar::OnAddAudioSource(MenuItem *)
{
    OnAddComponent<AudioSource>();
}

void MenuBar::OnAddNewBehaviour(MenuItem *)
{
    Path behaviourSourcePath = CreateNewBehaviour();
    BehaviourContainer *behaviourContainer =
        OnAddComponent<BehaviourContainer>();
    behaviourContainer->SetSourceFilepath(behaviourSourcePath);
}

void MenuBar::OnAddEmptyBehaviour(MenuItem *)
{
    BehaviourContainer *behaviourContainer =
        OnAddComponent<BehaviourContainer>();
    BANG_UNUSED(behaviourContainer);
}

void MenuBar::OnAddExistingBehaviour(MenuItem *)
{
    bool accepted;
    Path behaviourPath;
    EditorDialog::GetAsset("Select an existing Behaviour...",
                           Extensions::GetSourceFileExtensions(),
                           &behaviourPath,
                           &accepted);

    if (accepted)
    {
        BehaviourContainer *behaviourContainer =
            OnAddComponent<BehaviourContainer>();
        behaviourContainer->SetSourceFilepath(behaviourPath);
    }
}

void MenuBar::OnAddCamera(MenuItem *)
{
    Camera *cam = OnAddComponent<Camera>();
    GameObjectFactory::CreateDefaultCameraInto(cam);
}

void MenuBar::OnAddBoxCollider(MenuItem *)
{
    OnAddComponent<BoxCollider>();
}

void MenuBar::OnAddCapsuleCollider(MenuItem *)
{
    OnAddComponent<CapsuleCollider>();
}

void MenuBar::OnAddSphereCollider(MenuItem *)
{
    OnAddComponent<SphereCollider>();
}

void MenuBar::OnAddMeshCollider(MenuItem *item)
{
    MeshCollider *meshCollider = OnAddComponent<MeshCollider>();
    if (MeshRenderer *meshRenderer =
            meshCollider->GetGameObject()->GetComponent<MeshRenderer>())
    {
        meshCollider->SetMesh(meshRenderer->GetActiveMesh());
    }
}

void MenuBar::OnAddPointLight(MenuItem *)
{
    OnAddComponent<PointLight>();
}

void MenuBar::OnAddDirectionalLight(MenuItem *)
{
    OnAddComponent<DirectionalLight>();
}

void MenuBar::OnAddLineRenderer(MenuItem *)
{
    OnAddComponent<LineRenderer>();
}

void MenuBar::OnAddMeshRenderer(MenuItem *)
{
    OnAddComponent<MeshRenderer>();
}

void MenuBar::OnAddSkinnedMeshRenderer(MenuItem *)
{
    OnAddComponent<SkinnedMeshRenderer>();
}

void MenuBar::OnAddWaterRenderer(MenuItem *)
{
    OnAddComponent<WaterRenderer>();
}

void MenuBar::OnAddParticleSystem(MenuItem *item)
{
    OnAddComponent<ParticleSystem>();
}

void MenuBar::OnAddReflectionProbe(MenuItem *)
{
    OnAddComponent<ReflectionProbe>();
}

void MenuBar::OnAddTransform(MenuItem *)
{
    OnAddComponent<Transform>();
}

void MenuBar::OnAddRectTransform(MenuItem *)
{
    OnAddComponent<RectTransform>();
}

void MenuBar::OnAddUIAutoFocuser(MenuItem *)
{
    OnAddComponent<UIAutoFocuser>();
}

void MenuBar::OnAddUIButton(MenuItem *)
{
    OnAddComponent<UIButton>();
}

void MenuBar::OnAddUIHorizontalLayout(MenuItem *)
{
    OnAddComponent<UIHorizontalLayout>();
}

void MenuBar::OnAddUIVerticalLayout(MenuItem *)
{
    OnAddComponent<UIVerticalLayout>();
}

void MenuBar::OnAddUILayoutElement(MenuItem *)
{
    OnAddComponent<UILayoutElement>();
}

void MenuBar::OnAddUILayoutIgnorer(MenuItem *)
{
    OnAddComponent<UILayoutIgnorer>();
}

void MenuBar::OnAddUIMask(MenuItem *)
{
    OnAddComponent<UIMask>();
}

void MenuBar::OnAddUIRectMask(MenuItem *)
{
    OnAddComponent<UIRectMask>();
}

void MenuBar::OnAddUISlider(MenuItem *)
{
    OnAddComponent<UISlider>();
}

void MenuBar::OnAddUIScrollPanel(MenuItem *)
{
    OnAddComponent<UIScrollPanel>();
}

void MenuBar::OnAddUITextRenderer(MenuItem *)
{
    OnAddComponent<UITextRenderer>();
}

void MenuBar::OnAddUIImageRenderer(MenuItem *)
{
    OnAddComponent<UIImageRenderer>();
}

void MenuBar::OnAddUICanvas(MenuItem *)
{
    OnAddComponent<UICanvas>();
}

void MenuBar::OnAddUICheckBox(MenuItem *)
{
    OnAddComponent<UICheckBox>();
}

void MenuBar::OnAddUIComboBox(MenuItem *)
{
    OnAddComponent<UIComboBox>();
}

void MenuBar::OnAddUIFocusable(MenuItem *)
{
    OnAddComponent<UIFocusable>();
}

void MenuBar::OnAddUIInputText(MenuItem *)
{
    OnAddComponent<UIInputText>();
}

void MenuBar::OnAddUILabel(MenuItem *)
{
    OnAddComponent<UILabel>();
}

void MenuBar::OnAddPostProcessEffect(MenuItem *)
{
    OnAddComponent<PostProcessEffect>();
}

void MenuBar::OnAddRope(MenuItem *)
{
    OnAddComponent<Rope>();
}

void MenuBar::OnAddCloth(MenuItem *)
{
    OnAddComponent<Cloth>();
}

void MenuBar::OnAddSSAO(MenuItem *)
{
    OnAddComponent<PostProcessEffectSSAO>();
}

void MenuBar::OnAddRigidBody(MenuItem *)
{
    OnAddComponent<RigidBody>();
}

void MenuBar::OnCreateEmpty(MenuItem *)
{
    GameObject *go = GameObjectFactory::CreateGameObject();
    go->SetName("Empty");
    MenuBar::OnEndCreateGameObjectFromMenuBar(go);
}

void MenuBar::OnCreateCone(MenuItem *)
{
    MenuBar::OnEndCreateGameObjectFromMenuBar(
        GameObjectFactory::CreateConeGameObject());
}

void MenuBar::OnCreateCube(MenuItem *)
{
    MenuBar::OnEndCreateGameObjectFromMenuBar(
        GameObjectFactory::CreateCubeGameObject());
}

void MenuBar::OnCreateCapsule(MenuItem *)
{
    MenuBar::OnEndCreateGameObjectFromMenuBar(
        GameObjectFactory::CreateCapsuleGameObject());
}

void MenuBar::OnCreateCylinder(MenuItem *item)
{
    MenuBar::OnEndCreateGameObjectFromMenuBar(
        GameObjectFactory::CreateCylinderGameObject());
}

void MenuBar::OnCreateSphere(MenuItem *)
{
    MenuBar::OnEndCreateGameObjectFromMenuBar(
        GameObjectFactory::CreateSphereGameObject());
}

void MenuBar::OnCreateCamera(MenuItem *)
{
    GameObject *camGameObject = GameObjectFactory::CreateGameObject();
    camGameObject->SetName("Camera");
    GameObjectFactory::CreateDefaultCameraInto(camGameObject);
    camGameObject->AddComponent<AudioListener>();
    MenuBar::OnEndCreateGameObjectFromMenuBar(camGameObject);
}

void MenuBar::OnCreateParticleSystemGO(MenuItem *item)
{
    GameObject *psGameObject = GameObjectFactory::CreateGameObject();
    psGameObject->SetName("ParticleSystem");
    psGameObject->AddComponent<ParticleSystem>();
    MenuBar::OnEndCreateGameObjectFromMenuBar(psGameObject);
}

void MenuBar::OnCreateDirectionalLightGO(MenuItem *)
{
    GameObject *dlGo = GameObjectFactory::CreateGameObject();
    dlGo->SetName("DirectionalLight");
    dlGo->AddComponent<DirectionalLight>();
    MenuBar::OnEndCreateGameObjectFromMenuBar(dlGo);
}

void MenuBar::OnCreatePointLightGO(MenuItem *)
{
    GameObject *plGo = GameObjectFactory::CreateGameObject();
    plGo->SetName("PointLight");
    plGo->AddComponent<PointLight>();
    MenuBar::OnEndCreateGameObjectFromMenuBar(plGo);
}

UICanvas *MenuBar::OnCreateUICanvasGO(MenuItem *)
{
    GameObject *uiGo = GameObjectFactory::CreateUIGameObject();
    uiGo->SetName("Canvas");
    UICanvas *canvas = uiGo->AddComponent<UICanvas>();
    MenuBar::OnEndCreateUIGameObjectFromMenuBar(uiGo);
    return canvas;
}

void MenuBar::OnCreateUIEmptyGO(MenuItem *)
{
    GameObject *uiGo = GameObjectFactory::CreateUIGameObject();
    uiGo->SetName("EmptyUI");
    MenuBar::OnEndCreateUIGameObjectFromMenuBar(uiGo);
}

void MenuBar::OnCreateUIImageGO(MenuItem *)
{
    GameObject *uiGo = GameObjectFactory::CreateUIGameObject();
    uiGo->SetName("Image");
    uiGo->AddComponent<UIImageRenderer>();
    MenuBar::OnEndCreateUIGameObjectFromMenuBar(uiGo);
}

void MenuBar::OnCreateUITextGO(MenuItem *)
{
    GameObject *uiGo = GameObjectFactory::CreateUIGameObject();
    uiGo->SetName("Text");
    UITextRenderer *text = uiGo->AddComponent<UITextRenderer>();
    text->SetContent("UIText");
    MenuBar::OnEndCreateUIGameObjectFromMenuBar(uiGo);
}

void MenuBar::OnAlignGameObjectWithView(MenuItem *)
{
    if (GameObject *go = Editor::GetSelectedGameObject())
    {
        if (Transform *tr = go->GetTransform())
        {
            EditorCamera *edCam =
                EditSceneGameObjects::GetInstance()->GetEditorCamera();
            MetaNode metaBefore = tr->GetMeta();

            tr->SetPosition(edCam->GetTransform()->GetPosition());
            tr->SetRotation(edCam->GetTransform()->GetRotation());

            UndoRedoManager::PushAction(
                new UndoRedoSerializableChange(tr, metaBefore, tr->GetMeta()));
        }
    }
}

void MenuBar::OnAlignViewWithGameObject(MenuItem *)
{
    if (GameObject *go = Editor::GetSelectedGameObject())
    {
        EditSceneGameObjects::GetInstance()
            ->GetEditorCamera()
            ->AlignViewWithGameObject(go);
    }
}

void MenuBar::OnCreatePlane(MenuItem *)
{
    MenuBar::OnEndCreateGameObjectFromMenuBar(
        GameObjectFactory::CreatePlaneGameObject());
}

void MenuBar::OnEndCreateGameObjectFromMenuBar(GameObject *go)
{
    GameObject *parentGo = Editor::GetSelectedGameObject();
    if (!parentGo)
    {
        parentGo = EditorSceneManager::GetOpenScene();
    }

    if (parentGo)
    {
        go->SetParent(parentGo, -1, true);

        UndoRedoManager::PushAction(new UndoRedoCreateGameObject(go));

        Editor::GetInstance()->SelectGameObject(go, false);
    }
}

void MenuBar::OnEndCreateUIGameObjectFromMenuBar(GameObject *uiGo)
{
    GameObject *parentGo = Editor::GetSelectedGameObject();
    if (!parentGo)
    {
        parentGo = EditorSceneManager::GetOpenScene();
    }

    if (parentGo)
    {
        uiGo->SetParent(parentGo);

        if (!UICanvas::GetActive(uiGo))
        {
            UICanvas *createdCanvas = OnCreateUICanvasGO(nullptr);
            GameObject *createdCanvasGo = createdCanvas->GetGameObject();
            uiGo->SetParent(createdCanvasGo);

            UndoRedoManager::PushAction(new UndoRedoCreateGameObject(uiGo));
        }
        Editor::GetInstance()->SelectGameObject(uiGo, false);
    }
}

Path MenuBar::CreateNewBehaviour()
{
    Path behaviourDir = Explorer::GetInstance()->GetCurrentPath();
    String behaviourName = "";
    do
    {
        behaviourName =
            Dialog::GetString("Specify Behaviour name...",
                              "Please, the name of the new Behaviour: ",
                              "NewBehaviour");

        if (behaviourName == "")
        {
            return Path::Empty();
        }
    } while (
        !BehaviourCreator::CanCreateNewBehaviour(behaviourDir, behaviourName));

    Path behaviourHeaderPath;
    Path behaviourSourcePath;
    BehaviourCreator::CreateNewBehaviour(behaviourDir,
                                         behaviourName,
                                         &behaviourHeaderPath,
                                         &behaviourSourcePath);

    return behaviourSourcePath;
}
