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

#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorIconManager.h"
#include "BangEditor/ExplorerItem.h"

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
    p_directionLabel = GameObjectFactory::CreateUILabel();
    p_directionLabel->GetText()->SetTextSize(9);
    p_directionLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::Right);

    GameObject *dirBar = p_directionLabel->GetGameObject();

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

    SetAsChild(toolBar);
    toolBar->SetAsChild(p_backButton->GetGameObject());
    toolBar->SetAsChild(dirBar);
    SetAsChild(GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 5));

    SetAsChild(p_scrollPanel->GetGameObject());

    p_scrollPanel->GetScrollArea()->SetContainedGameObject(p_itemsContainer);
    p_scrollPanel->SetVerticalShowScrollMode(ShowScrollMode::WhenNeeded);
    p_scrollPanel->SetVerticalScrollBarSide(HorizontalSide::Right);
    p_scrollPanel->SetHorizontalScrollEnabled(false);
}

Explorer::~Explorer()
{
}

void Explorer::SetCurrentPath(const Path &path)
{
    if (GetCurrentPath() != path)
    {
        m_currentPath = path;
        p_directionLabel->GetText()->SetContent(m_currentPath.GetAbsolute());

        Clear();
        List<Path> subPaths = m_currentPath.FindSubPaths(Path::FindFlag::Simple);
        for (const Path &subPath : subPaths)
        {
            AddItem(subPath);
        }
        p_scrollPanel->SetScrolling(Vector2i::Zero);
    }
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

void Explorer::AddItem(const Path &itemPath)
{
    ExplorerItem *explorerItem = GameObject::Create<ExplorerItem>();
    explorerItem->SetFilepath(itemPath);
    p_itemsContainer->SetAsChild(explorerItem);

    p_items.PushBack(explorerItem);
}

void Explorer::GoDirectoryUp()
{
    SetCurrentPath( GetCurrentPath().GetDirectory() );
}

Explorer *Explorer::GetInstance()
{
    return EditorScene::GetInstance()->GetExplorer();
}
