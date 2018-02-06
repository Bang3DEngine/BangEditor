#include "BangEditor/EditorDialog.h"

#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Dialog.h"
#include "Bang/UILabel.h"
#include "Bang/UIButton.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIGridLayout.h"
#include "Bang/RectTransform.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/ExplorerItem.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

bool EditorDialog::s_accepted = false;
Path EditorDialog::s_assetPathResult = Path::Empty;

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

    DialogWindow *dialog = Dialog::BeginCreateDialog(title, 500, 400, true);

    EditorDialog::s_accepted = false;
    EditorDialog::s_assetPathResult = Path::Empty;

    Scene *scene = GameObjectFactory::CreateScene(false);
    EditorDialog::CreateGetAssetSceneInto(scene, extensions);
    SceneManager::LoadSceneInstantly(scene, false);

    Dialog::EndCreateDialog(dialog);
    *resultPath = EditorDialog::s_assetPathResult;
    *accepted = EditorDialog::s_accepted;
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

    UIButton *cancelButton = GameObjectFactory::CreateUIButton();
    cancelButton->GetText()->SetContent("Cancel");
    cancelButton->GetFocusable()->AddClickedCallback([](IFocusable*)
    {
        EditorDialog::s_accepted = false;
        EditorDialog::s_assetPathResult = Path::Empty;
        Dialog::EndCurrentDialog();
    });


    UIButton *openButton = GameObjectFactory::CreateUIButton();
    openButton->GetText()->SetContent("Open");
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


