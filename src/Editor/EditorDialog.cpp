#include "BangEditor/EditorDialog.h"

#include "Bang/Paths.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Dialog.h"
#include "Bang/Window.h"
#include "Bang/Camera.h"
#include "Bang/UILabel.h"
#include "Bang/Material.h"
#include "Bang/UIButton.h"
#include "Bang/UISlider.h"
#include "Bang/GameObject.h"
#include "Bang/TextureFactory.h"
#include "Bang/DialogWindow.h"
#include "Bang/SceneManager.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIGridLayout.h"
#include "Bang/RectTransform.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"
#include "Bang/ShaderProgramFactory.h"

#include "BangEditor/UITabHeader.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/ExplorerItem.h"
#include "BangEditor/UITabContainer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

bool EditorDialog::s_accepted = false;
Path EditorDialog::s_assetPathResult = Path::EmptyPath();

EditorDialog::EditorDialog()
{
}

EditorDialog::~EditorDialog()
{
}

void EditorDialog::GetAsset(const String &title,
                            const Array<String> &extensions,
                            Path *resultPath,
                            bool *accepted)
{
    ASSERT(resultPath);
    ASSERT(accepted);

    EditorDialog::s_accepted = false;
    EditorDialog::s_assetPathResult = Path::Empty;

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
    EditorDialog::CreateGetColorSceneInto(scene, initialColor, colorPickerReporter);
    Dialog::EndDialogCreation(scene);
}


Scene *EditorDialog::CreateGetAssetSceneInto(Scene *scene,
                                             const Array<String> &extensions)
{
    GameObjectFactory::CreateUISceneInto(scene);

    scene->GetCamera()->SetClearColor( Color::White.WithValue(0.7f) );

    UIVerticalLayout *vl = scene->AddComponent<UIVerticalLayout>();
    vl->SetPaddings(5);
    vl->SetSpacing(10);

    auto CreateAssetContainerGameObject = [scene, &extensions]
                                          (const List<Path> &assetPaths) ->
                                          GameObject*
    {
        UIScrollPanel *gridScrollPanel = GameObjectFactory::CreateUIScrollPanel();
        gridScrollPanel->GetScrollArea()->GetBackground()->SetTint(Color::Zero);

        GameObject *gridScrollPanelGo = gridScrollPanel->GetGameObject();
        UILayoutElement *spLE = gridScrollPanelGo->AddComponent<UILayoutElement>();
        spLE->SetFlexibleSize( Vector2::One );

        GameObject *gridLayoutGo = GameObjectFactory::CreateUIGameObject();

        UIContentSizeFitter *csf = gridLayoutGo->AddComponent<UIContentSizeFitter>();
        csf->SetHorizontalSizeType(LayoutSizeType::NONE);
        csf->SetVerticalSizeType(LayoutSizeType::PREFERRED);

        gridLayoutGo->GetRectTransform()->SetPivotPosition(Vector2(-1,1));
        UIGridLayout *gridLayout = gridLayoutGo->AddComponent<UIGridLayout>();
        gridLayout->SetCellSize( Vector2i(80) );
        gridLayout->SetSpacing(10);

        UILayoutElement *gridLE = gridLayoutGo->AddComponent<UILayoutElement>();
        gridLE->SetFlexibleSize( Vector2::One );

        gridScrollPanelGo->SetParent(scene);
        gridScrollPanel->GetScrollArea()->SetContainedGameObject(gridLayoutGo);
        gridScrollPanel->SetVerticalShowScrollMode(ShowScrollMode::WHEN_NEEDED);
        gridScrollPanel->SetVerticalScrollBarSide(HorizontalSide::RIGHT);
        gridScrollPanel->SetHorizontalScrollEnabled(false);

        // Add paths to grid layout
        for (const Path &assetPath : assetPaths)
        {
            ExplorerItem *expItem = GameObject::Create<ExplorerItem>();
            expItem->GetLabel()->GetText()->SetTextColor(Color::Black);
            expItem->SetPath(assetPath);

            if (assetPath.IsEmpty())
            {
                expItem->GetLabel()->GetText()->SetContent("None");
            }

            expItem->GetFocusable()->AddClickedCallback(
            [expItem, gridLayoutGo](IFocusable*, ClickType clickType)
            {
                if (clickType == ClickType::DOWN)
                {
                    // Save path, and select only the clicked one
                    EditorDialog::s_assetPathResult = expItem->GetPath();
                    for (GameObject *expItemGo : gridLayoutGo->GetChildren())
                    {
                        ExplorerItem *expItem = DCAST<ExplorerItem*>(expItemGo);
                        if (expItem) { expItem->SetSelected(false); }
                    }
                    expItem->SetSelected(true);
                }
                else if (clickType == ClickType::DOUBLE)
                {
                    // Directly select
                    EditorDialog::s_accepted = true;
                    Dialog::EndCurrentDialog();
                }
            });

            expItem->SetParent(gridLayoutGo);
        }

        return gridScrollPanel->GetGameObject();
    };

    // Add tabs
    UITabContainer *tabContainer = GameObject::Create<UITabContainer>();

    List<Path> engineAssetPaths =
            Paths::GetEngineAssetsDir().GetFiles(Path::FindFlag::RECURSIVE,
                                            extensions);
    engineAssetPaths.PushFront(Path::Empty);
    GameObject *engineAssetsGo = CreateAssetContainerGameObject(engineAssetPaths);

    List<Path> projectAssetPaths =
            Paths::GetProjectAssetsDir().GetFiles(Path::FindFlag::RECURSIVE,
                                                  extensions);
    projectAssetPaths.PushFront(Path::Empty);
    GameObject *projectAssetsGo = CreateAssetContainerGameObject(projectAssetPaths);

    UILayoutElement *tabContainerLE = tabContainer->AddComponent<UILayoutElement>();
    tabContainerLE->SetFlexibleSize(Vector2::One);

    tabContainer->AddTab("Project", projectAssetsGo);
    tabContainer->AddTab("Engine", engineAssetsGo);
    tabContainer->SetParent(scene);

    // Buttons below
    GameObject *buttonsGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *buttonsHL = buttonsGo->AddComponent<UIHorizontalLayout>();
    buttonsHL->SetSpacing(5);

    UIButton *cancelButton = GameObjectFactory::CreateUIButton("Cancel");
    cancelButton->AddClickedCallback([]()
    {
        EditorDialog::s_accepted = false;
        EditorDialog::s_assetPathResult = Path::Empty;
        Dialog::EndCurrentDialog();
    });

    UIButton *openButton = GameObjectFactory::CreateUIButton("Open");
    openButton->AddClickedCallback([]()
    {
        EditorDialog::s_accepted = true;
        Dialog::EndCurrentDialog();
    });

    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::FLEXIBLE, 1.0f)
                       ->SetParent(buttonsGo);
    cancelButton->GetGameObject()->SetParent(buttonsGo);
    openButton->GetGameObject()->SetParent(buttonsGo);
    buttonsGo->SetParent(scene);

    return scene;
}

Scene *EditorDialog::CreateGetColorSceneInto(Scene *scene,
                                             const Color &initialColor,
                                             ColorPickerReporter *colorPickerReporter)
{
    GameObjectFactory::CreateUISceneInto(scene);
    scene->GetCamera()->SetClearColor( Color::White.WithValue(0.7f) );

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
    colorVLLE->SetPreferredSize( Vector2i(250, 350) );
    colorVLLE->SetFlexibleSize(Vector2::Zero);
    colorVL->SetPaddingBot(5);
    colorVL->SetSpacing(10);
    colorVLGo->SetParent(mainHLGo);

    UIImageRenderer *colorPanelImg = GameObjectFactory::CreateUIImage();
    UIFocusable *colorPanelFocusable = colorPanelImg->GetGameObject()->
                                              AddComponent<UIFocusable>();
    colorPanelFocusable->SetCursorType(Cursor::Type::HAND);
    UILayoutElement *colorPanelImgLE = colorPanelImg->GetGameObject()->
                                              AddComponent<UILayoutElement>();
    colorPanelImgLE->SetFlexibleSize( Vector2::One );
    colorPanelImg->GetGameObject()->SetParent(colorVLGo);

    Path edShadersPath = EditorPaths::GetEditorAssetsDir().Append("Shaders");
    RH<ShaderProgram> colorPanelImgSP;
    colorPanelImgSP.Set( ShaderProgramFactory::Get(
                        EPATH("Shaders/UIImageRenderer.vert"),
                        edShadersPath.Append("UIInputColor.frag")) );
    colorPanelImg->GetMaterial()->SetShaderProgram(colorPanelImgSP.Get());

    UISlider *hueSlider = GameObjectFactory::CreateUISlider();
    hueSlider->GetInputNumber()->GetGameObject()->SetEnabled(false);
    UILayoutElement *hueSliderLE = hueSlider->GetGameObject()->
                                    AddComponent<UILayoutElement>();
    hueSliderLE->SetPreferredHeight(30);
    hueSlider->SetMinMaxValues(0.0f, 1.0f);
    hueSlider->GetGameObject()->SetParent(colorVLGo);

    RH<ShaderProgram> hueImgSP;
    hueImgSP.Set( ShaderProgramFactory::Get(
                        EPATH("Shaders/UIImageRenderer.vert"),
                        edShadersPath.Append("UIInputColorOnlyHue.frag")) );
    UIImageRenderer *hueSliderGuide = hueSlider->GetGuideRenderer();
    hueSliderGuide->GetMaterial()->SetShaderProgram(hueImgSP.Get());
    hueSliderGuide->GetGameObject()->GetRectTransform()->SetAnchors(-Vector2::One,
                                                                     Vector2::One);

    GameObject *controlsVLGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *controlsVL = controlsVLGo->AddComponent<UIVerticalLayout>();
    UILayoutElement *controlsVLLE = controlsVLGo->AddComponent<UILayoutElement>();
    controlsVLLE->SetFlexibleWidth(1.0f);
    controlsVL->SetSpacing(5);
    controlsVL->SetPaddings(5);
    controlsVLGo->SetParent(mainHLGo);

    auto CreateRGBSlider = [](const String &label,
                              const Color &labelColor) -> UISlider*
    {
        GameObject *sliderHLGo = GameObjectFactory::CreateUIGameObject();
        UIHorizontalLayout *sliderHL = sliderHLGo->AddComponent<UIHorizontalLayout>();
        sliderHL->SetSpacing(10);

        UILabel *sliderLabel = GameObjectFactory::CreateUILabel();
        sliderLabel->GetText()->SetContent(label);
        sliderLabel->GetText()->SetTextColor(labelColor);
        UILayoutElement *sliderLabelLE = sliderLabel->GetGameObject()->
                                               GetComponent<UILayoutElement>();
        sliderLabelLE->SetFlexibleWidth(0.0f);
        sliderLabel->GetGameObject()->SetParent(sliderHLGo);

        UISlider *slider = GameObjectFactory::CreateUISlider();
        slider->SetMinMaxValues(0.0f, 1.0f);
        slider->GetGameObject()->SetParent(sliderHLGo);

        return slider;
    };
    UISlider *sliderR = CreateRGBSlider("R: ", Color::Red);
    UISlider *sliderG = CreateRGBSlider("G: ", Color::Green.WithValue(0.3f));
    UISlider *sliderB = CreateRGBSlider("B: ", Color::Blue);
    UISlider *sliderA = CreateRGBSlider("A: ", Color::Black);
    sliderR->SetValue(initialColor.r);
    sliderG->SetValue(initialColor.g);
    sliderB->SetValue(initialColor.b);
    sliderA->SetValue(initialColor.a);
    sliderR->GetGameObject()->GetParent()->SetParent(controlsVLGo);
    sliderG->GetGameObject()->GetParent()->SetParent(controlsVLGo);
    sliderB->GetGameObject()->GetParent()->SetParent(controlsVLGo);
    sliderA->GetGameObject()->GetParent()->SetParent(controlsVLGo);

    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::PREFERRED,
                                       10.0f)->SetParent(controlsVLGo);

    UILabel *resultLabel = GameObjectFactory::CreateUILabel();
    resultLabel->GetGameObject()->GetComponent<UILayoutElement>()->
                 SetFlexibleHeight(0.0f);
    resultLabel->GetText()->SetContent("Result:");
    resultLabel->GetText()->SetTextColor(Color::Black);
    resultLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::LEFT);
    resultLabel->GetGameObject()->SetParent(controlsVLGo);

    UIImageRenderer *resultColorContainer = GameObjectFactory::CreateUIImage();
    UILayoutElement *resultColorContLE = resultColorContainer ->GetGameObject()->
                                               AddComponent<UILayoutElement>();
    resultColorContLE->SetFlexibleHeight(1.0f);
    resultColorContainer->SetImageTexture(TextureFactory::GetCheckerboard().Get());
    resultColorContainer->GetGameObject()->SetParent(controlsVLGo);

    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::PREFERRED,
                                       35.0f)->SetParent(controlsVLGo);

    UIImageRenderer *resultColor = GameObjectFactory::CreateUIImage();
    resultColor->GetGameObject()->SetParent(resultColorContainer->GetGameObject());

    class Handle : public GameObject,
                   public EventEmitter<IValueChangedListener>
    {
    public:
        UIImageRenderer *p_img = nullptr;
        bool m_colorPanelHasBeenPressed = false;

        Handle()
        {
            GameObjectFactory::CreateUIGameObjectInto(this);

            UIFocusable *focusable = AddComponent<UIFocusable>();
            focusable->SetCursorType( Cursor::Type::HAND );

            p_img = AddComponent<UIImageRenderer>();
            p_img->SetImageTexture(TextureFactory::GetCircleIcon().Get());
            p_img->GetGameObject()->GetRectTransform()->SetMargins(-5);
            p_img->GetGameObject()->GetRectTransform()->SetAnchors(Vector2::Zero);
        }

        Vector2 GetPositionRelativeToColorPanel() const
        {
            return GetRectTransform()->GetAnchorMin();
        }

        void Update() override
        {
            GameObject::Update();

            RectTransform *parentRT = GetParent()->GetRectTransform();
            if ((parentRT->IsMouseOver() || GetRectTransform()->IsMouseOver()) &&
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
                Vector2 mouseCoordsAnchor = parentRT->
                            FromViewportPointToLocalPointNDC(mouseCoordsVP);
                mouseCoordsAnchor = Vector2::Clamp(mouseCoordsAnchor,
                                                   -Vector2::One, Vector2::One);
                p_img->GetGameObject()->GetRectTransform()->
                                         SetAnchors(mouseCoordsAnchor);
                EventEmitter<IValueChangedListener>::PropagateToListeners(
                            &IValueChangedListener::OnValueChanged, this);
            }
        }

    };

    Handle *handle = GameObject::Create<Handle>();
    handle->SetParent(colorPanelImg->GetGameObject());

    class Controller : public GameObject,
                       public EventListener<IValueChangedListener>
    {
    public:
        Color m_pickedColorRGB;
        Color m_pickedColorHSV;

        UISlider *p_sliderRGB_R;
        UISlider *p_sliderRGB_G;
        UISlider *p_sliderRGB_B;
        UISlider *p_sliderRGB_A;
        UISlider *p_sliderHue;
        Handle   *p_handle;
        ShaderProgram *p_colorImgSP;
        UIImageRenderer *p_resultImg;
        ColorPickerReporter *p_colorPickerReporter;

        void OnValueChanged(Object *object) override
        {
            EventListener<IValueChangedListener>::SetReceiveEvents(false);

            if (object == p_sliderHue)
            {
                m_pickedColorHSV.h = p_sliderHue->GetValue();
                m_pickedColorRGB = m_pickedColorHSV.ToRGB();
            }
            else if (object == p_handle)
            {
                Vector2 sv = p_handle->GetPositionRelativeToColorPanel() * 0.5f + 0.5f;
                m_pickedColorHSV.s = sv[0];
                m_pickedColorHSV.v = sv[1];
                m_pickedColorRGB = m_pickedColorHSV.ToRGB();
            }
            else
            {
                m_pickedColorRGB =  Color(p_sliderRGB_R->GetValue(),
                                          p_sliderRGB_G->GetValue(),
                                          p_sliderRGB_B->GetValue());

                // Correct for undeterminate hsv configs, keeping the previous
                // known values
                Color prevColorHSV = m_pickedColorHSV;
                m_pickedColorHSV = m_pickedColorRGB.ToHSV();
                if (m_pickedColorHSV.s <= 0.0f)
                {
                    m_pickedColorHSV.h = prevColorHSV.h;
                    m_pickedColorHSV.v = prevColorHSV.v;
                }

                if (m_pickedColorHSV.v <= 0.01f)
                {
                    m_pickedColorHSV.h = prevColorHSV.h;
                    m_pickedColorHSV.s = prevColorHSV.s;
                    m_pickedColorHSV.v = 0.0f;
                }
            }
            m_pickedColorRGB.a = m_pickedColorHSV.a = p_sliderRGB_A->GetValue();

            // Update controls
            p_sliderRGB_R->SetValue(m_pickedColorRGB.r);
            p_sliderRGB_G->SetValue(m_pickedColorRGB.g);
            p_sliderRGB_B->SetValue(m_pickedColorRGB.b);
            p_sliderRGB_A->SetValue(m_pickedColorRGB.a);
            p_sliderHue->SetValue( m_pickedColorHSV.h );
            Vector2 handlePos = Vector2(m_pickedColorHSV.s, m_pickedColorHSV.v) *
                                2.0f - 1.0f;
            p_handle->GetRectTransform()->SetAnchors(handlePos);

            p_resultImg->SetTint(m_pickedColorRGB);

            GL::Push(GL::BindTarget::SHADER_PROGRAM);

            p_colorImgSP->Bind();
            p_colorImgSP->SetFloat("B_Hue", p_sliderHue->GetValue());

            GL::Pop(GL::BindTarget::SHADER_PROGRAM);

            p_colorPickerReporter->SetPickedColor( m_pickedColorRGB );
            EventListener<IValueChangedListener>::SetReceiveEvents(true);
        }

        void OnDestroy() override
        {
            GameObject::OnDestroy();
            p_colorPickerReporter->SetHasFinished(true);
        }
    };

    Controller *controller = GameObject::Create<Controller>();
    controller->p_colorImgSP          = colorPanelImgSP.Get();
    controller->p_sliderHue           = hueSlider;
    controller->p_sliderRGB_R         = sliderR;
    controller->p_sliderRGB_G         = sliderG;
    controller->p_sliderRGB_B         = sliderB;
    controller->p_sliderRGB_A         = sliderA;
    controller->p_handle              = handle;
    controller->p_resultImg           = resultColor;
    controller->m_pickedColorRGB      = initialColor;
    controller->m_pickedColorHSV      = initialColor.ToHSV();
    controller->p_colorPickerReporter = colorPickerReporter;
    hueSlider->EventEmitter<IValueChangedListener>::RegisterListener(controller);
    handle->EventEmitter<IValueChangedListener>::RegisterListener(controller);
    sliderR->EventEmitter<IValueChangedListener>::RegisterListener(controller);
    sliderG->EventEmitter<IValueChangedListener>::RegisterListener(controller);
    sliderB->EventEmitter<IValueChangedListener>::RegisterListener(controller);
    sliderA->EventEmitter<IValueChangedListener>::RegisterListener(controller);
    controller->OnValueChanged(controller->p_sliderRGB_R); // Update controls
    controller->SetParent(scene);

    return scene;
}



void ColorPickerReporter::SetPickedColor(const Color &color)
{
    if (color != GetPickedColor())
    {
        m_pickedColor = color;
        EventEmitter<IValueChangedListener>::PropagateToListeners(
                    &IValueChangedListener::OnValueChanged, this);
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
