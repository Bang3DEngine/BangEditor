#include "BangEditor/EditorDialog.h"

#include "Bang/Alignment.h"
#include "Bang/Assert.h"
#include "Bang/Camera.h"
#include "Bang/Cursor.h"
#include "Bang/Dialog.h"
#include "Bang/EventListener.h"
#include "Bang/GL.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsFocus.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Input.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/Material.h"
#include "Bang/MouseButton.h"
#include "Bang/Path.h"
#include "Bang/Paths.h"
#include "Bang/RectTransform.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Resources.h"
#include "Bang/Scene.h"
#include "Bang/ShaderProgram.h"
#include "Bang/ShaderProgramFactory.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIButton.h"
#include "Bang/UIContentSizeFitter.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIGridLayout.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UILabel.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UISlider.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/Vector2.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/ExplorerItem.h"
#include "BangEditor/ExplorerItemFactory.h"
#include "BangEditor/NavigatorItem.h"
#include "BangEditor/ObjectItem.h"
#include "BangEditor/UITabContainer.h"

using namespace Bang;
using namespace BangEditor;

bool EditorDialog::s_accepted = false;
Path EditorDialog::s_assetPathResult = Path::Empty();
Object *EditorDialog::s_objectResult = nullptr;

EditorDialog::EditorDialog()
{
}

EditorDialog::~EditorDialog()
{
}

void EditorDialog::GetObject(const String &title,
                             ClassIdType acceptedClassIdBegin,
                             ClassIdType acceptedClassIdEnd,
                             Object **resultObject,
                             bool *accepted)
{
    ASSERT(resultObject);
    ASSERT(accepted);

    Scene *openScene = EditorSceneManager::GetActive()->GetOpenScene();
    EditorDialog::s_accepted = false;
    EditorDialog::s_assetPathResult = Path::Empty();

    Dialog::BeginDialogCreation(title, 500, 400, true, true);
    Scene *scene = GameObjectFactory::CreateScene(false);
    EditorDialog::CreateGetObjectSceneInto(
        scene, acceptedClassIdBegin, acceptedClassIdEnd, openScene);
    Dialog::EndDialogCreation(scene);

    *resultObject = EditorDialog::s_objectResult;
    *accepted = EditorDialog::s_accepted;
}

void EditorDialog::GetAsset(const String &title,
                            const Array<String> &extensions,
                            Path *resultPath,
                            bool *accepted)
{
    ASSERT(resultPath);
    ASSERT(accepted);

    EditorDialog::s_accepted = false;
    EditorDialog::s_assetPathResult = Path::Empty();

    Dialog::BeginDialogCreation(title, 500, 400, true, true);
    Scene *scene = GameObjectFactory::CreateScene(false);
    EditorDialog::CreateGetAssetSceneInto(scene, extensions);
    Dialog::EndDialogCreation(scene);

    *resultPath = EditorDialog::s_assetPathResult;
    *accepted = EditorDialog::s_accepted;
}

void EditorDialog::GetColor(const String &title,
                            const Color &initialColor,
                            ColorPickerReporter *colorPickerReporter)
{
    ASSERT(colorPickerReporter);
    colorPickerReporter->SetHasFinished(false);
    colorPickerReporter->SetPickedColor(initialColor);
    Dialog::BeginDialogCreation(title, 550, 400, true, false);
    Scene *scene = GameObjectFactory::CreateScene(false);
    EditorDialog::CreateGetColorSceneInto(
        scene, initialColor, colorPickerReporter);
    Dialog::EndDialogCreation(scene);
}

void EditorDialog::CreateSearchSceneInto(
    Scene *scene,
    const Array<String> &tabNames,
    const Array<Array<NavigatorItem *>> &tabNavItems,
    std::function<void(NavigatorItem *)> OnNavigatorItemSelected)
{
    GameObjectFactory::CreateUISceneInto(scene);

    scene->GetCamera()->SetClearColor(Color::White().WithValue(0.7f));

    UIVerticalLayout *vl = scene->AddComponent<UIVerticalLayout>();
    vl->SetPaddings(5);
    vl->SetSpacing(10);

    // Create tabs and their content
    UITabContainer *tabContainer = new UITabContainer();
    UILayoutElement *tabContainerLE =
        tabContainer->AddComponent<UILayoutElement>();
    tabContainerLE->SetFlexibleSize(Vector2::One());

    for (uint i = 0; i < tabNames.Size(); ++i)
    {
        const String &tabName = tabNames[i];
        const Array<NavigatorItem *> &navItems = tabNavItems[i];

        UIScrollPanel *gridScrollPanel =
            GameObjectFactory::CreateUIScrollPanel();
        gridScrollPanel->GetScrollArea()->GetBackground()->SetTint(
            Color::Zero());

        GameObject *gridScrollPanelGo = gridScrollPanel->GetGameObject();
        UILayoutElement *spLE =
            gridScrollPanelGo->AddComponent<UILayoutElement>();
        spLE->SetFlexibleSize(Vector2::One());

        GameObject *gridLayoutGo = GameObjectFactory::CreateUIGameObject();

        UIContentSizeFitter *csf =
            gridLayoutGo->AddComponent<UIContentSizeFitter>();
        csf->SetHorizontalSizeType(LayoutSizeType::NONE);
        csf->SetVerticalSizeType(LayoutSizeType::PREFERRED);

        gridLayoutGo->GetRectTransform()->SetPivotPosition(Vector2(-1, 1));
        UIGridLayout *gridLayout = gridLayoutGo->AddComponent<UIGridLayout>();
        gridLayout->SetCellSize(Vector2i(100));
        gridLayout->SetPaddings(10);
        gridLayout->SetSpacing(10);

        UILayoutElement *gridLE = gridLayoutGo->AddComponent<UILayoutElement>();
        gridLE->SetFlexibleSize(Vector2::One());

        gridScrollPanelGo->SetParent(scene);
        gridScrollPanel->GetScrollArea()->SetContainedGameObject(gridLayoutGo);
        gridScrollPanel->SetVerticalShowScrollMode(ShowScrollMode::WHEN_NEEDED);
        gridScrollPanel->SetVerticalScrollBarSide(HorizontalSide::RIGHT);
        gridScrollPanel->SetHorizontalScrollEnabled(false);

        // Add paths to grid layout
        for (NavigatorItem *navItem : navItems)
        {
            navItem->GetFocusable()->AddEventCallback(
                [navItem, gridLayoutGo, OnNavigatorItemSelected](
                    UIFocusable *, const UIEvent &event) {
                    if (event.type == UIEvent::Type::MOUSE_CLICK_DOWN)
                    {
                        for (GameObject *itemGo : gridLayoutGo->GetChildren())
                        {
                            if (NavigatorItem *neigborNavItem =
                                    DCAST<NavigatorItem *>(itemGo))
                            {
                                neigborNavItem->SetSelected(false);
                            }
                        }
                        navItem->SetSelected(true);
                        OnNavigatorItemSelected(navItem);
                        return UIEventResult::INTERCEPT;
                    }
                    else if (event.type == UIEvent::Type::MOUSE_CLICK_DOUBLE)
                    {
                        EditorDialog::s_accepted = true;
                        Dialog::EndCurrentDialog();
                        return UIEventResult::INTERCEPT;
                    }
                    return UIEventResult::IGNORE;
                });

            navItem->SetParent(gridLayoutGo);
        }

        tabContainer->AddTab(tabName, gridScrollPanelGo);
    }
    tabContainer->SetParent(scene);

    // Buttons below
    GameObject *buttonsGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *buttonsHL =
        buttonsGo->AddComponent<UIHorizontalLayout>();
    buttonsHL->SetSpacing(5);

    UIButton *cancelButton = GameObjectFactory::CreateUIButton("Cancel");
    cancelButton->AddClickedCallback([]() {
        EditorDialog::s_accepted = false;
        EditorDialog::s_assetPathResult = Path::Empty();
        Dialog::EndCurrentDialog();
    });

    UIButton *openButton = GameObjectFactory::CreateUIButton("Open");
    openButton->AddClickedCallback([]() {
        EditorDialog::s_accepted = true;
        Dialog::EndCurrentDialog();
    });

    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::FLEXIBLE, 1.0f)
        ->SetParent(buttonsGo);
    cancelButton->GetGameObject()->SetParent(buttonsGo);
    openButton->GetGameObject()->SetParent(buttonsGo);
    buttonsGo->SetParent(scene);
}

void EditorDialog::CreateGetObjectSceneInto(Scene *scene,
                                            ClassIdType acceptedClassIdBegin,
                                            ClassIdType acceptedClassIdEnd,
                                            GameObject *baseGO)
{
    Map<String, Array<NavigatorItem *>> tabsContent;

    Array<GameObject *> tabsBaseGameObjects = {baseGO};

    Array<String> tabsNames;
    Array<Array<NavigatorItem *>> tabsNavItems;
    for (uint i = 0; i < tabsBaseGameObjects.Size(); ++i)
    {
        const GameObject *baseGameObject = tabsBaseGameObjects[i];
        const String tabName = baseGameObject->GetName();

        Array<NavigatorItem *> navItems;
        {
            Array<Object *> objects;
            {
                objects.PushBack(nullptr);

                Array<GameObject *> children =
                    baseGameObject->GetChildrenRecursively();
                objects.PushBack(children);

                for (GameObject *child : children)
                {
                    Array<Component *> comps = child->GetComponents();
                    objects.PushBack(comps);
                }
            }

            for (Object *object : objects)
            {
                if (!object ||
                    ClassDB::IsSubClass(
                        acceptedClassIdBegin, acceptedClassIdEnd, object))
                {
                    ObjectItem *objectItem = new ObjectItem();
                    objectItem->SetObject(object);
                    navItems.PushBack(objectItem);
                }
            }
        }
        tabsNames.PushBack(tabName);
        tabsNavItems.PushBack(navItems);
    }

    CreateSearchSceneInto(
        scene, tabsNames, tabsNavItems, [](NavigatorItem *navItem) {
            ObjectItem *objItem = SCAST<ObjectItem *>(navItem);
            EditorDialog::s_objectResult = objItem->GetObject();
        });
}

void EditorDialog::CreateGetAssetSceneInto(Scene *scene,
                                           const Array<String> &extensions)
{
    Map<String, Array<NavigatorItem *>> tabsContent;

    Array<String> tabsNames = {"Project", "Engine"};
    Array<Path> tabsBasePaths = {Paths::GetProjectAssetsDir(),
                                 Paths::GetEngineAssetsDir()};

    Array<Array<NavigatorItem *>> tabsNavItems;
    for (uint i = 0; i < tabsNames.Size(); ++i)
    {
        const Path &basePath = tabsBasePaths[i];
        Array<ExplorerItem *> expItems =
            ExplorerItemFactory::CreateAndGetSubPathsExplorerItems(
                basePath, false, true);

        ExplorerItem *noneExpItem = new ExplorerItem();
        noneExpItem->SetPath(Path::Empty());
        expItems.PushFront(noneExpItem);

        Array<NavigatorItem *> navItems;
        for (ExplorerItem *expItem : expItems)
        {
            Path path = expItem->GetPath();
            if (path.IsEmpty() || path.HasExtension(extensions))
            {
                String textContent = path.GetNameExt();
                if (path.IsEmpty())
                {
                    textContent = "None";
                }
                else
                {
                    if (Resources::IsEmbeddedResource(path))
                    {
                        textContent = path.GetDirectory().GetNameExt() + "/" +
                                      path.GetNameExt();
                    }
                }
                expItem->GetLabel()->GetText()->SetContent(textContent);
                expItem->GetLabel()->GetText()->SetTextColor(Color::Black());

                navItems.PushBack(expItem);
            }
            else
            {
                GameObject::Destroy(expItem);
            }
        }
        tabsNavItems.PushBack(navItems);
    }

    CreateSearchSceneInto(
        scene, tabsNames, tabsNavItems, [](NavigatorItem *navItem) {
            ExplorerItem *expItem = SCAST<ExplorerItem *>(navItem);
            EditorDialog::s_assetPathResult = expItem->GetPath();
        });
}

void EditorDialog::CreateGetColorSceneInto(
    Scene *scene,
    const Color &initialColor,
    ColorPickerReporter *colorPickerReporter)
{
    GameObjectFactory::CreateUISceneInto(scene);
    scene->GetCamera()->SetClearColor(Color::White().WithValue(0.7f));

    UIVerticalLayout *mainVL = scene->AddComponent<UIVerticalLayout>();
    mainVL->SetPaddings(40);
    mainVL->SetSpacing(10);

    GameObject *mainHLGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *mainHL = mainHLGo->AddComponent<UIHorizontalLayout>();
    mainHL->SetPaddingBot(5);
    mainHL->SetSpacing(10);
    mainHLGo->SetParent(mainVL->GetGameObject());

    GameObject *colorVLGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *colorVL = colorVLGo->AddComponent<UIVerticalLayout>();
    UILayoutElement *colorVLLE = colorVLGo->AddComponent<UILayoutElement>();
    colorVLLE->SetPreferredSize(Vector2i(250, 350));
    colorVLLE->SetFlexibleSize(Vector2::Zero());
    colorVL->SetPaddingBot(5);
    colorVL->SetSpacing(10);
    colorVLGo->SetParent(mainHLGo);

    UIImageRenderer *colorPanelImg = GameObjectFactory::CreateUIImage();
    UIFocusable *colorPanelFocusable =
        colorPanelImg->GetGameObject()->AddComponent<UIFocusable>();
    colorPanelFocusable->SetCursorType(Cursor::Type::HAND);
    UILayoutElement *colorPanelImgLE =
        colorPanelImg->GetGameObject()->AddComponent<UILayoutElement>();
    colorPanelImgLE->SetFlexibleSize(Vector2::One());
    colorPanelImg->GetGameObject()->SetParent(colorVLGo);

    Path edShadersPath = EditorPaths::GetEditorAssetsDir().Append("Shaders");
    RH<ShaderProgram> colorPanelImgSP;
    colorPanelImgSP.Set(ShaderProgramFactory::Get(
        EPATH("Shaders").Append("UIImageRenderer.vert"),
        edShadersPath.Append("UIInputColor.frag")));
    colorPanelImg->GetMaterial()->SetShaderProgram(colorPanelImgSP.Get());

    UISlider *hueSlider = GameObjectFactory::CreateUISlider();
    hueSlider->GetInputNumber()->GetGameObject()->SetEnabled(false);
    UILayoutElement *hueSliderLE =
        hueSlider->GetGameObject()->AddComponent<UILayoutElement>();
    hueSliderLE->SetPreferredHeight(30);
    hueSlider->SetMinMaxValues(0.0f, 1.0f);
    hueSlider->GetGameObject()->SetParent(colorVLGo);

    RH<ShaderProgram> hueImgSP;
    hueImgSP.Set(ShaderProgramFactory::Get(
        EPATH("Shaders/UIImageRenderer.vert"),
        edShadersPath.Append("UIInputColorOnlyHue.frag")));
    UIImageRenderer *hueSliderGuide = hueSlider->GetGuideRenderer();
    hueSliderGuide->GetMaterial()->SetShaderProgram(hueImgSP.Get());
    hueSliderGuide->GetGameObject()->GetRectTransform()->SetAnchors(
        -Vector2::One(), Vector2::One());

    GameObject *controlsVLGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *controlsVL =
        controlsVLGo->AddComponent<UIVerticalLayout>();
    UILayoutElement *controlsVLLE =
        controlsVLGo->AddComponent<UILayoutElement>();
    controlsVLLE->SetFlexibleWidth(1.0f);
    controlsVL->SetSpacing(5);
    controlsVL->SetPaddings(5);
    controlsVLGo->SetParent(mainHLGo);

    auto CreateRGBSlider = [](const String &label,
                              const Color &labelColor) -> UISlider * {
        GameObject *sliderHLGo = GameObjectFactory::CreateUIGameObject();
        UIHorizontalLayout *sliderHL =
            sliderHLGo->AddComponent<UIHorizontalLayout>();
        sliderHL->SetSpacing(10);

        UILabel *sliderLabel = GameObjectFactory::CreateUILabel();
        sliderLabel->GetText()->SetContent(label);
        sliderLabel->GetText()->SetTextColor(labelColor);
        UILayoutElement *sliderLabelLE =
            sliderLabel->GetGameObject()->GetComponent<UILayoutElement>();
        sliderLabelLE->SetFlexibleWidth(0.0f);
        sliderLabel->GetGameObject()->SetParent(sliderHLGo);

        UISlider *slider = GameObjectFactory::CreateUISlider();
        slider->SetMinMaxValues(0.0f, 1.0f);
        slider->GetGameObject()->SetParent(sliderHLGo);

        return slider;
    };
    UISlider *sliderR = CreateRGBSlider("R: ", Color::Red());
    UISlider *sliderG = CreateRGBSlider("G: ", Color::Green().WithValue(0.3f));
    UISlider *sliderB = CreateRGBSlider("B: ", Color::Blue());
    UISlider *sliderA = CreateRGBSlider("A: ", Color::Black());
    sliderR->SetValue(initialColor.r);
    sliderG->SetValue(initialColor.g);
    sliderB->SetValue(initialColor.b);
    sliderA->SetValue(initialColor.a);
    sliderR->GetGameObject()->GetParent()->SetParent(controlsVLGo);
    sliderG->GetGameObject()->GetParent()->SetParent(controlsVLGo);
    sliderB->GetGameObject()->GetParent()->SetParent(controlsVLGo);
    sliderA->GetGameObject()->GetParent()->SetParent(controlsVLGo);

    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::PREFERRED, 10.0f)
        ->SetParent(controlsVLGo);

    UILabel *resultLabel = GameObjectFactory::CreateUILabel();
    resultLabel->GetGameObject()
        ->GetComponent<UILayoutElement>()
        ->SetFlexibleHeight(0.0f);
    resultLabel->GetText()->SetContent("Result:");
    resultLabel->GetText()->SetTextColor(Color::Black());
    resultLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::LEFT);
    resultLabel->GetGameObject()->SetParent(controlsVLGo);

    UIImageRenderer *resultColorContainer = GameObjectFactory::CreateUIImage();
    UILayoutElement *resultColorContLE =
        resultColorContainer->GetGameObject()->AddComponent<UILayoutElement>();
    resultColorContLE->SetFlexibleHeight(1.0f);
    resultColorContainer->SetImageTexture(TextureFactory::GetCheckerboard());
    resultColorContainer->GetGameObject()->SetParent(controlsVLGo);

    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::PREFERRED, 35.0f)
        ->SetParent(controlsVLGo);

    UIImageRenderer *resultColor = GameObjectFactory::CreateUIImage();
    resultColor->GetGameObject()->SetParent(
        resultColorContainer->GetGameObject());

    class Handle : public GameObject, public EventEmitter<IEventsValueChanged>
    {
    public:
        UIImageRenderer *p_img = nullptr;
        bool m_colorPanelHasBeenPressed = false;

        Handle()
        {
            GameObjectFactory::CreateUIGameObjectInto(this);

            UIFocusable *focusable = AddComponent<UIFocusable>();
            focusable->SetCursorType(Cursor::Type::HAND);

            p_img = AddComponent<UIImageRenderer>();
            p_img->SetImageTexture(TextureFactory::GetCircleIcon());
            p_img->GetGameObject()->GetRectTransform()->SetMargins(-5);
            p_img->GetGameObject()->GetRectTransform()->SetAnchors(
                Vector2::Zero());
        }

        Vector2 GetPositionRelativeToColorPanel() const
        {
            return GetRectTransform()->GetAnchorMin();
        }

        void Update() override
        {
            GameObject::Update();

            RectTransform *parentRT = GetParent()->GetRectTransform();
            if ((parentRT->IsMouseOver() ||
                 GetRectTransform()->IsMouseOver()) &&
                Input::GetMouseButton(MouseButton::LEFT))
            {
                m_colorPanelHasBeenPressed = true;
            }

            if (Input::GetMouseButtonUp(MouseButton::LEFT))
            {
                m_colorPanelHasBeenPressed = false;
            }

            if (m_colorPanelHasBeenPressed)
            {
                Vector2i mouseCoordsVP = Input::GetMousePosition();
                Vector2 mouseCoordsAnchor =
                    parentRT->FromViewportPointToLocalPointNDC(mouseCoordsVP);
                mouseCoordsAnchor = Vector2::Clamp(
                    mouseCoordsAnchor, -Vector2::One(), Vector2::One());
                p_img->GetGameObject()->GetRectTransform()->SetAnchors(
                    mouseCoordsAnchor);
                EventEmitter<IEventsValueChanged>::PropagateToListeners(
                    &IEventsValueChanged::OnValueChanged, this);
            }
        }
    };

    Handle *handle = new Handle();
    handle->SetParent(colorPanelImg->GetGameObject());

    class Controller : public GameObject,
                       public EventListener<IEventsValueChanged>
    {
    public:
        Color m_pickedColorRGB;
        Color m_pickedColorHSV;

        UISlider *p_sliderRGB_R;
        UISlider *p_sliderRGB_G;
        UISlider *p_sliderRGB_B;
        UISlider *p_sliderRGB_A;
        UISlider *p_sliderHue;
        Handle *p_handle;
        ShaderProgram *p_colorImgSP;
        UIImageRenderer *p_resultImg;
        ColorPickerReporter *p_colorPickerReporter;

        void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override
        {
            EventListener<IEventsValueChanged>::SetReceiveEvents(false);

            if (object == p_sliderHue)
            {
                m_pickedColorHSV.r = p_sliderHue->GetValue();
                m_pickedColorRGB = m_pickedColorHSV.ToRGB();
            }
            else if (object == p_handle)
            {
                Vector2 sv =
                    p_handle->GetPositionRelativeToColorPanel() * 0.5f + 0.5f;
                m_pickedColorHSV.g = sv[0];
                m_pickedColorHSV.b = sv[1];
                m_pickedColorRGB = m_pickedColorHSV.ToRGB();
            }
            else
            {
                m_pickedColorRGB = Color(p_sliderRGB_R->GetValue(),
                                         p_sliderRGB_G->GetValue(),
                                         p_sliderRGB_B->GetValue());

                // Correct for undeterminate hsv configs, keeping the
                // previous
                // known values
                Color prevColorHSV = m_pickedColorHSV;
                m_pickedColorHSV = m_pickedColorRGB.ToHSV();
                if (m_pickedColorHSV.g <= 0.0f)
                {
                    m_pickedColorHSV.r = prevColorHSV.r;
                    m_pickedColorHSV.b = prevColorHSV.b;
                }

                if (m_pickedColorHSV.b <= 0.01f)
                {
                    m_pickedColorHSV.r = prevColorHSV.r;
                    m_pickedColorHSV.g = prevColorHSV.g;
                    m_pickedColorHSV.b = 0.0f;
                }
            }
            m_pickedColorRGB.a = m_pickedColorHSV.a = p_sliderRGB_A->GetValue();

            // Update controls
            p_sliderRGB_R->SetValue(m_pickedColorRGB.r);
            p_sliderRGB_G->SetValue(m_pickedColorRGB.g);
            p_sliderRGB_B->SetValue(m_pickedColorRGB.b);
            p_sliderRGB_A->SetValue(m_pickedColorRGB.a);
            p_sliderHue->SetValue(m_pickedColorHSV.r);
            Vector2 handlePos =
                Vector2(m_pickedColorHSV.g, m_pickedColorHSV.b) * 2.0f - 1.0f;
            p_handle->GetRectTransform()->SetAnchors(handlePos);

            p_resultImg->SetTint(m_pickedColorRGB);

            GL::Push(GL::BindTarget::SHADER_PROGRAM);

            p_colorImgSP->Bind();
            p_colorImgSP->SetFloat("B_Hue", p_sliderHue->GetValue());

            GL::Pop(GL::BindTarget::SHADER_PROGRAM);

            p_colorPickerReporter->SetIsPicking(true);
            p_colorPickerReporter->SetPickedColor(m_pickedColorRGB);
            EventListener<IEventsValueChanged>::SetReceiveEvents(true);
        }

        void OnDestroy() override
        {
            GameObject::OnDestroy();
            p_colorPickerReporter->SetIsPicking(false);
            p_colorPickerReporter->SetHasFinished(true);
        }
    };

    Controller *controller = new Controller();
    controller->p_colorImgSP = colorPanelImgSP.Get();
    controller->p_sliderHue = hueSlider;
    controller->p_sliderRGB_R = sliderR;
    controller->p_sliderRGB_G = sliderG;
    controller->p_sliderRGB_B = sliderB;
    controller->p_sliderRGB_A = sliderA;
    controller->p_handle = handle;
    controller->p_resultImg = resultColor;
    controller->m_pickedColorRGB = initialColor;
    controller->m_pickedColorHSV = initialColor.ToHSV();
    controller->p_colorPickerReporter = colorPickerReporter;
    hueSlider->EventEmitter<IEventsValueChanged>::RegisterListener(controller);
    handle->EventEmitter<IEventsValueChanged>::RegisterListener(controller);
    sliderR->EventEmitter<IEventsValueChanged>::RegisterListener(controller);
    sliderG->EventEmitter<IEventsValueChanged>::RegisterListener(controller);
    sliderB->EventEmitter<IEventsValueChanged>::RegisterListener(controller);
    sliderA->EventEmitter<IEventsValueChanged>::RegisterListener(controller);
    controller->OnValueChanged(controller->p_sliderRGB_R);  // Update controls
    controller->SetParent(scene);
}

void ColorPickerReporter::SetPickedColor(const Color &color)
{
    if (color != GetPickedColor())
    {
        m_pickedColor = color;
        EventEmitter<IEventsValueChanged>::PropagateToListeners(
            &IEventsValueChanged::OnValueChanged, this);
    }
}

void ColorPickerReporter::SetHasFinished(bool hasFinished)
{
    if (hasFinished != HasFinished())
    {
        m_hasFinished = hasFinished;
    }
}

Color ColorPickerReporter::GetPickedColor() const
{
    return m_pickedColor;
}

bool ColorPickerReporter::HasFinished() const
{
    return m_hasFinished;
}

bool ColorPickerReporter::IsPicking() const
{
    return m_isPicking;
}

void ColorPickerReporter::SetIsPicking(bool isPicking)
{
    m_isPicking = isPicking;
}
