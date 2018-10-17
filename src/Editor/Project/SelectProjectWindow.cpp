#include "BangEditor/SelectProjectWindow.h"

#include "Bang/Alignment.h"
#include "Bang/Array.tcc"
#include "Bang/AspectRatioMode.h"
#include "Bang/Camera.h"
#include "Bang/Color.h"
#include "Bang/Dialog.h"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/RectTransform.h"
#include "Bang/Scene.h"
#include "Bang/Texture2D.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIAspectRatioFitter.h"
#include "Bang/UIButton.h"
#include "Bang/UICanvas.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILabel.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIList.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/WindowManager.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorSettings.h"
#include "BangEditor/Project.h"
#include "BangEditor/ProjectManager.h"

namespace Bang
{
template <class>
class Array;
}

using namespace Bang;
using namespace BangEditor;

Path SelectProjectWindow::SelectedProjectPath = Path::EmptyPath();

SelectProjectWindow::SelectProjectWindow()
{
}

SelectProjectWindow::~SelectProjectWindow()
{
}

void SelectProjectWindow::Init()
{
    SetTitle("Bang Editor - Select Project");
    SetMinSize(500, 500);
    SetMaxSize(500, 500);

    Scene *scene = GameObject::Create<SelectProjectScene>();
    EditorSceneManager::LoadScene(scene, true);
}

// ===========================================================================

SelectProjectScene::SelectProjectScene()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    GameObject *cameraGo = GameObjectFactory::CreateGameObject(true);
    Camera *camera = GameObjectFactory::CreateUICameraInto(cameraGo);
    camera->GetGameObject()->SetParent(this);
    SetCamera(camera);

    UICanvas *canvas = GameObjectFactory::CreateUICanvas();
    GameObject *canvasGo = canvas->GetGameObject();
    UIImageRenderer *bg = canvasGo->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::One.WithValue(0.75f));
    canvas->GetGameObject()->SetParent(this);

    GameObject *mainVLGo = GameObjectFactory::CreateUIGameObject();
    UIVerticalLayout *mainVL = mainVLGo->AddComponent<UIVerticalLayout>();
    mainVL->SetPaddings(10);
    mainVL->SetSpacing(10);
    mainVLGo->SetParent(canvasGo);

    GameObject *logoContainer = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *logoLE = logoContainer->AddComponent<UILayoutElement>();
    logoLE->SetFlexibleSize(Vector2(1, 0.5f));
    logoLE->SetLayoutPriority(-5);
    logoContainer->SetParent(mainVLGo);

    UIImageRenderer *logo = GameObjectFactory::CreateUIImage();
    Texture2D *logoTex = TextureFactory::GetBang2048Icon();
    logo->SetImageTexture(logoTex);
    GameObject *logoGo = logo->GetGameObject();
    logoGo->GetRectTransform()->SetAnchorX(Vector2(0));
    logoGo->GetRectTransform()->SetAnchorY(Vector2(0));
    logoGo->GetRectTransform()->SetPivotPosition(Vector2(0, 0));
    UIAspectRatioFitter *logoARF = logoGo->AddComponent<UIAspectRatioFitter>();
    logoARF->SetAspectRatio(logoTex->GetSize());
    logoARF->SetAspectRatioMode(AspectRatioMode::KEEP);
    logo->GetGameObject()->SetParent(logoContainer);

    GameObjectFactory::CreateUIHSeparator()->SetParent(mainVLGo);

    UILabel *recentPLLabel = GameObjectFactory::CreateUILabel();
    UILayoutElement *recentPLLabelLE =
        recentPLLabel->GetGameObject()->GetComponent<UILayoutElement>();
    recentPLLabelLE->SetFlexibleSize(Vector2::Zero);
    recentPLLabel->GetText()->SetContent("Recent projects:");
    recentPLLabel->GetText()->SetTextColor(Color::Black);
    recentPLLabel->GetText()->SetTextSize(16);
    recentPLLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::LEFT);
    recentPLLabel->GetGameObject()->SetParent(mainVLGo);

    GameObject *recentPLContainer = GameObjectFactory::CreateUIGameObject(true);

    UILayoutElement *rplLE = recentPLContainer->AddComponent<UILayoutElement>();
    rplLE->SetFlexibleSize(Vector2(1, 1));
    recentPLContainer->SetParent(mainVLGo);

    UIList *recentProjectsList = GameObjectFactory::CreateUIList(true);
    recentProjectsList->SetSelectionCallback([this](GOItem *item,
                                                    UIList::Action action) {
        if(!item)
        {
            return;
        }
        RecentProjectListEntry *entry = SCAST<RecentProjectListEntry *>(item);
        switch(action)
        {
            case UIList::Action::MOUSE_LEFT_DOWN:
                m_selectedRecentPath = entry->m_projectPath;
                break;

            case UIList::Action::DOUBLE_CLICKED_LEFT:
                ConfirmOpenProject(entry->m_projectPath);
                break;

            default: break;
        }
    });
    GameObject *recentProjectsListGo = recentProjectsList->GetGameObject();
    recentProjectsListGo->SetParent(recentPLContainer);
    UIScrollPanel *rplSP = recentProjectsList->GetScrollPanel();
    rplSP->SetForceHorizontalFit(true);
    rplSP->SetHorizontalScrollBarSide(VerticalSide::BOT);
    rplSP->SetVerticalScrollBarSide(HorizontalSide::RIGHT);
    rplSP->SetVerticalShowScrollMode(ShowScrollMode::WHEN_NEEDED);
    rplSP->SetHorizontalShowScrollMode(ShowScrollMode::WHEN_NEEDED);
    recentProjectsList->Clear();
    const Array<Path> &recentProjects =
        EditorSettings::GetRecentProjectFilepathsOpen();
    for(const Path &recentProjectPath : recentProjects)
    {
        GameObject *entry =
            GameObject::Create<RecentProjectListEntry>(recentProjectPath);
        recentProjectsList->AddItem(entry);
    }

    GameObjectFactory::CreateUIHSeparator()->SetParent(mainVLGo);

    GameObject *botHLGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *botHL = botHLGo->AddComponent<UIHorizontalLayout>();
    botHL->SetSpacing(20);
    UILayoutElement *botHLLE = botHLGo->AddComponent<UILayoutElement>();
    botHLLE->SetMinHeight(30);
    botHLLE->SetFlexibleSize(Vector2(1, 0));
    botHLGo->SetParent(mainVLGo);

    GameObjectFactory::CreateUIHSpacer()->SetParent(botHLGo);

    p_newProjectButton = GameObjectFactory::CreateUIButton("New project...");
    p_newProjectButton->GetGameObject()->SetParent(botHLGo);
    p_newProjectButton->AddClickedCallback([this]() { NewProject(); });

    p_openProjectButton = GameObjectFactory::CreateUIButton("Open project...");
    p_openProjectButton->GetGameObject()->SetParent(botHLGo);
    p_openProjectButton->AddClickedCallback([this]() { OpenProject(); });

    p_openSelectedProjectButton =
        GameObjectFactory::CreateUIButton("Open selected project...");
    p_openSelectedProjectButton->GetGameObject()->SetParent(botHLGo);
    p_openSelectedProjectButton->AddClickedCallback(
        [this]() { ConfirmOpenProject(m_selectedRecentPath); });
}

SelectProjectScene::~SelectProjectScene()
{
}

void SelectProjectScene::Update()
{
    Scene::Update();
    bool somethingSelected = !m_selectedRecentPath.IsEmpty();
    p_openSelectedProjectButton->SetBlocked(!somethingSelected);
}

void SelectProjectScene::NewProject()
{
    Path newProjectDirPath = Dialog::OpenDirectory(
        "Create new project. Select parent dir...", EditorPaths::GetHome());
    if(newProjectDirPath.IsDir())
    {
        String projectName = Dialog::GetString(
            "Choose Project Name", "Please, choose your project name:",
            "NewProject");
        if(!projectName.IsEmpty())
        {
            Project *proj = ProjectManager::CreateNewProject(newProjectDirPath,
                                                             projectName);
            ConfirmOpenProject(proj->GetProjectFilepath());
        }
    }
}

void SelectProjectScene::OpenProject()
{
    Path projectFilepath = Dialog::OpenFilePath(
        "Open Project...", {Extensions::GetProjectExtension()},
        EditorPaths::GetHome());
    if(projectFilepath.IsFile())
    {
        ConfirmOpenProject(projectFilepath);
    }
}

void SelectProjectScene::ConfirmOpenProject(const Path &projectFilepath)
{
    SelectProjectWindow::SelectedProjectPath = projectFilepath;
    WindowManager::GetInstance()->DestroyWindow(Window::GetActive());
}

// ===========================================================================

SelectProjectScene::RecentProjectListEntry::RecentProjectListEntry()
{
}

SelectProjectScene::RecentProjectListEntry::RecentProjectListEntry(
    const Path &projectPath)
    : GameObject()
{
    m_projectPath = projectPath;

    GameObjectFactory::CreateUIGameObjectInto(this);
    AddComponent<UIFocusable>();

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetPaddings(5);

    GameObject *container = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *text = container->AddComponent<UITextRenderer>();
    text->SetContent(projectPath.GetAbsolute());
    text->SetTextSize(12);
    text->SetTextColor(Color::Black);
    text->SetHorizontalAlign(HorizontalAlignment::LEFT);

    container->SetParent(this);
}

SelectProjectScene::RecentProjectListEntry::~RecentProjectListEntry()
{
}
