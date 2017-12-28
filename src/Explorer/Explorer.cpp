#include "BangEditor/Explorer.h"

#include "Bang/Paths.h"
#include "Bang/UILabel.h"
#include "Bang/UIButton.h"
#include "Bang/UIGridLayout.h"
#include "Bang/UIScrollArea.h"
#include "Bang/RectTransform.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

#include "BangEditor/Editor.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/ExplorerItem.h"
#include "BangEditor/EditorIconManager.h"
#include "BangEditor/EditorSceneManager.h"


USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Explorer::Explorer() : EditorUITab("Explorer")
{
    UILayoutElement *le = GetComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(100) );
    le->SetPreferredWidth(250);

    // Tool Bar
    GameObject *toolBar = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *toolBarHL = toolBar->AddComponent<UIHorizontalLayout>();
    toolBarHL->SetPaddingLeft(3);
    toolBarHL->SetPaddingRight( toolBarHL->GetPaddingLeft() );
    toolBarHL->SetSpacing(3);

    // Scroll Panel
    p_scrollPanel = GameObjectFactory::CreateUIScrollPanel();
    p_scrollPanel->GetScrollArea()->GetBackground()->SetTint(Color::Zero);
    GameObject *scrollPanelGo = p_scrollPanel->GetGameObject();
    UILayoutElement *spLE = scrollPanelGo->AddComponent<UILayoutElement>();
    spLE->SetFlexibleSize( Vector2::One );

    // Back button
    p_backButton = GameObjectFactory::CreateUIButton("", nullptr);
    RH<Texture2D> backButtonTex = EditorIconManager::GetBackArrowIcon();
    p_backButton->SetIcon(backButtonTex.Get(), Vector2i(15), 0);
    p_backButton->GetText()->SetContent("");
    p_backButton->GetButton()->AddClickedCallback( [this](IFocusable*)
    { GoDirectoryUp(); });

    UILayoutElement *toolBarLE = toolBar->AddComponent<UILayoutElement>();
    toolBarLE->SetMinHeight(15);
    toolBarLE->SetFlexibleHeight(0);

    // Direction label
    p_currentPathLabel = GameObjectFactory::CreateUILabel();
    p_currentPathLabel->GetText()->SetTextSize(9);
    p_currentPathLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::Right);

    GameObject *dirBar = p_currentPathLabel->GetGameObject();

    // Items Container
    p_itemsContainer = GameObjectFactory::CreateUIGameObject();
    p_itemsContainer->GetRectTransform()->SetPivotPosition(Vector2(-1,1));

    UIContentSizeFitter *csf = p_itemsContainer->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::None);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);

    UIGridLayout *gridLayout = p_itemsContainer->AddComponent<UIGridLayout>();
    gridLayout->SetCellSize( Vector2i(80) );
    gridLayout->SetSpacing(10);

    SetCurrentPath( Paths::EngineAssets() );

    toolBar->SetParent(this);
    p_backButton->GetGameObject()->SetParent(toolBar);
    dirBar->SetParent(toolBar);
    GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 5)->SetParent(this);

    p_scrollPanel->GetGameObject()->SetParent(this);

    p_scrollPanel->GetScrollArea()->SetContainedGameObject(p_itemsContainer);
    p_scrollPanel->SetVerticalShowScrollMode(ShowScrollMode::WhenNeeded);
    p_scrollPanel->SetVerticalScrollBarSide(HorizontalSide::Right);
    p_scrollPanel->SetHorizontalScrollEnabled(false);

    ProjectManager::GetInstance()->
            EventEmitter<ProjectManagerListener>::RegisterListener(this);
}

Explorer::~Explorer()
{
}

void Explorer::SetRootPath(const Path &rootPath)
{
    if (rootPath != GetRootPath())
    {
        m_rootPath = rootPath;
        if (!IsInsideRootPath( GetCurrentPath() ))
        {
            SetCurrentPath( GetRootPath() );
        }
    }
}

void Explorer::SetCurrentPath(const Path &path)
{
    if (GetCurrentPath() != path && IsInsideRootPath(path))
    {
        m_currentPath = path;
        p_currentPathLabel->GetText()->SetContent(m_currentPath.GetAbsolute());

        p_backButton->SetBlocked( GetCurrentPath() == GetRootPath() );

        Clear();
        List<Path> subPaths = m_currentPath.FindSubPaths(Path::FindFlag::Simple);
        for (const Path &subPath : subPaths)
        {
            AddItem(subPath);
        }
        p_scrollPanel->SetScrolling(Vector2i::Zero);
    }
}

const Path &Explorer::GetRootPath() const
{
    return m_rootPath;
}

const Path &Explorer::GetCurrentPath() const
{
    return m_currentPath;
}

void Explorer::Clear()
{
    for (ExplorerItem *explorerItem : p_items)
    {
        GameObject::Destroy(explorerItem);
    }
    p_items.Clear();
}

void Explorer::OnProjectOpen(const Project *project)
{
    ProjectManagerListener::OnProjectOpen(project);
    SetRootPath(EditorPaths::ProjectAssets());
    SetCurrentPath(EditorPaths::ProjectAssets());
}

void Explorer::OnProjectClosed(const Project *project)
{
    ProjectManagerListener::OnProjectClosed(project);
    SetCurrentPath(Paths::EngineAssets());
}

void Explorer::AddItem(const Path &itemPath)
{
    ExplorerItem *explorerItem = GameObject::Create<ExplorerItem>();
    explorerItem->SetFilepath(itemPath);
    explorerItem->SetParent(p_itemsContainer);

    explorerItem->GetButton()->AddClickedCallback(&Explorer::OnItemSelected);
    explorerItem->GetButton()->AddDoubleClickedCallback(&Explorer::OnItemDoubleClicked);

    p_items.PushBack(explorerItem);
}

void Explorer::GoDirectoryUp()
{
    SetCurrentPath( GetCurrentPath().GetDirectory() );
}

void Explorer::OnItemSelected(IFocusable *itemFocusable)
{
    GameObject *itemGo = Cast<UIFocusable*>(itemFocusable)->GetGameObject();
    ExplorerItem *expItem = Cast<ExplorerItem*>(itemGo);

    Editor::OnPathSelected(expItem->GetPath());
}

void Explorer::OnItemDoubleClicked(IFocusable *itemFocusable)
{
    GameObject *itemGo = Cast<UIFocusable*>(itemFocusable)->GetGameObject();
    ExplorerItem *expItem = Cast<ExplorerItem*>(itemGo);
    ASSERT(expItem);

    if (expItem->GetPath().IsDir())
    {
        Explorer::GetInstance()->SetCurrentPath(expItem->GetPath());
    }
}

bool Explorer::IsInsideRootPath(const Path &path) const
{
    return path.GetAbsolute().BeginsWith( GetRootPath().GetAbsolute() );
}

Explorer *Explorer::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetExplorer();
}
