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
#include "Bang/IconManager.h"
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

#include "BangEditor/EditorPaths.h"
#include "BangEditor/ExplorerItem.h"

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

    UIVerticalLayout *vl = scene->AddComponent<UIVerticalLayout>();
    vl->SetPaddings(5);
    vl->SetSpacing(10);

    UIScrollPanel *gridScrollPanel = GameObjectFactory::CreateUIScrollPanel();
    gridScrollPanel->GetScrollArea()->GetBackground()->SetTint(Color::Zero);

    GameObject *gridScrollPanelGo = gridScrollPanel->GetGameObject();
    UILayoutElement *spLE = gridScrollPanelGo->AddComponent<UILayoutElement>();
    spLE->SetFlexibleSize( Vector2::One );

    GameObject *gridLayoutGo = GameObjectFactory::CreateUIGameObject();

    UIContentSizeFitter *csf = gridLayoutGo->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::None);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);

    gridLayoutGo->GetRectTransform()->SetPivotPosition(Vector2(-1,1));
    UIGridLayout *gridLayout = gridLayoutGo->AddComponent<UIGridLayout>();
    gridLayout->SetCellSize( Vector2i(80) );
    gridLayout->SetSpacing(10);

    UILayoutElement *gridLE = gridLayoutGo->AddComponent<UILayoutElement>();
    gridLE->SetFlexibleSize( Vector2::One );

    GameObject *buttonsGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *buttonsHL = buttonsGo->AddComponent<UIHorizontalLayout>();
    buttonsHL->SetSpacing(5);

    UIButton *cancelButton = GameObjectFactory::CreateUIButton("Cancel");
    cancelButton->GetFocusable()->AddClickedCallback([](IFocusable*)
    {
        EditorDialog::s_accepted = false;
        EditorDialog::s_assetPathResult = Path::Empty;
        Dialog::EndCurrentDialog();
    });


    UIButton *openButton = GameObjectFactory::CreateUIButton("Open");
    openButton->GetFocusable()->AddClickedCallback([](IFocusable*)
    {
        EditorDialog::s_accepted = true;
        Dialog::EndCurrentDialog();
    });

    gridScrollPanelGo->SetParent(scene);
    gridScrollPanel->GetScrollArea()->SetContainedGameObject(gridLayoutGo);
    gridScrollPanel->SetVerticalShowScrollMode(ShowScrollMode::WhenNeeded);
    gridScrollPanel->SetVerticalScrollBarSide(HorizontalSide::Right);
    gridScrollPanel->SetHorizontalScrollEnabled(false);

    buttonsGo->SetParent(scene);
    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::Flexible, 1.0f)
                       ->SetParent(buttonsGo);
    cancelButton->GetGameObject()->SetParent(buttonsGo);
    openButton->GetGameObject()->SetParent(buttonsGo);

    // Find asset paths with extensions, and add them to grid layout
    List<Path> foundAssetPaths;
    List<Path> engineAssetPaths =
            Paths::GetEngineAssetsDir().GetFiles(Path::FindFlag::Recursive,
                                            extensions);
    List<Path> projectAssetPaths =
            Paths::GetProjectAssetsDir().GetFiles(Path::FindFlag::Recursive,
                                                  extensions);
    foundAssetPaths.PushBack(engineAssetPaths);
    foundAssetPaths.PushBack(projectAssetPaths);
    foundAssetPaths.PushFront(Path::Empty);

    for (const Path &assetPath : foundAssetPaths)
    {
        ExplorerItem *expItem = GameObject::Create<ExplorerItem>();
        expItem->GetLabel()->GetText()->SetTextColor(Color::White);
        expItem->SetPath(assetPath);

        if (assetPath.IsEmpty())
        {
            expItem->GetLabel()->GetText()->SetContent("None");
        }

        expItem->GetFocusable()->AddClickedCallback([expItem](IFocusable*)
        {
            EditorDialog::s_assetPathResult = expItem->GetPath();
        });
        expItem->GetFocusable()->AddDoubleClickedCallback([](IFocusable*)
        {
            EditorDialog::s_accepted = true;
            Dialog::EndCurrentDialog();
        });
        expItem->SetParent(gridLayoutGo);
    }

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
    colorPanelFocusable->SetCursorType(Cursor::Type::Hand);
    UILayoutElement *colorPanelImgLE = colorPanelImg->GetGameObject()->
                                              AddComponent<UILayoutElement>();
    colorPanelImgLE->SetFlexibleSize( Vector2::One );
    colorPanelImg->GetGameObject()->SetParent(colorVLGo);

    Path edShadersPath = EditorPaths::GetEditorAssetsDir().Append("Shaders");
    RH<ShaderProgram> colorPanelImgSP;
    colorPanelImgSP.Set( ShaderProgramFactory::Get(
                        EPATH("Shaders/UI/G_UIImageRenderer.vert"),
                        edShadersPath.Append("G_UIInputColor.frag")) );
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
                        EPATH("Shaders/UI/G_UIImageRenderer.vert"),
                        edShadersPath.Append("G_UIInputColorOnlyHue.frag")) );
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

    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::Preferred,
                                       10.0f)->SetParent(controlsVLGo);

    UILabel *resultLabel = GameObjectFactory::CreateUILabel();
    resultLabel->GetGameObject()->GetComponent<UILayoutElement>()->
                 SetFlexibleHeight(0.0f);
    resultLabel->GetText()->SetContent("Result:");
    resultLabel->GetText()->SetTextColor(Color::Black);
    resultLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    resultLabel->GetGameObject()->SetParent(controlsVLGo);

    UIImageRenderer *resultColorContainer = GameObjectFactory::CreateUIImage();
    UILayoutElement *resultColorContLE = resultColorContainer ->GetGameObject()->
                                               AddComponent<UILayoutElement>();
    resultColorContLE->SetFlexibleHeight(1.0f);
    resultColorContainer->SetImageTexture(IconManager::GetCheckerboard().Get());
    resultColorContainer->GetGameObject()->SetParent(controlsVLGo);

    GameObjectFactory::CreateUIVSpacer(LayoutSizeType::Preferred,
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
            focusable->SetCursorType( Cursor::Type::Hand );

            p_img = AddComponent<UIImageRenderer>();
            p_img->SetImageTexture(IconManager::GetCircleIcon().Get());
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
                  Input::GetMouseButton(MouseButton::Left))
            {
                m_colorPanelHasBeenPressed = true;
            }

            if (Input::GetMouseButtonUp(MouseButton::Left))
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
                       public IValueChangedListener
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
            SetReceiveEvents(false);

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

            GLId prevBoundSP = GL::GetBoundId(p_colorImgSP->GetGLBindTarget());
            p_colorImgSP->Bind();
            p_colorImgSP->Set("B_Hue", p_sliderHue->GetValue());
            GL::Bind(p_colorImgSP->GetGLBindTarget(), prevBoundSP);

            p_colorPickerReporter->SetPickedColor( m_pickedColorRGB );
            SetReceiveEvents(true);
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